#include "../cmn/win32.hpp"
#include "../tcatlib/api.hpp"
#include "cancel.hpp"

namespace cancel {

class keyMonitor : public iKeyMonitor {
public:
   keyMonitor();
   ~keyMonitor();

   virtual void install(bool add);

   virtual void waitForCancelUntil(HANDLE h, std::function<void(void)> f);
   virtual void clearAborted() { m_evt.clear(); }

private:
   void onCancel() { m_evt.set(); }

   cmn::osEvent m_evt;

   static BOOL WINAPI onHandleThunk(DWORD ctrl);
   static int gRefCnt;
   static keyMonitor *gHandler;
};

int keyMonitor::gRefCnt = 0;
keyMonitor *keyMonitor::gHandler = NULL;

keyMonitor::keyMonitor()
: m_evt("",/*auto*/false)
{
   if(gRefCnt != 0 || gHandler != NULL)
      throw std::runtime_error("ISE");
   gHandler = this;
}

keyMonitor::~keyMonitor()
{
   gHandler = NULL;
   while(gRefCnt > 0)
      install(/*add*/ false);
}

void keyMonitor::install(bool add)
{
   if(add)
   {
      gRefCnt++;
      auto success = ::SetConsoleCtrlHandler(&onHandleThunk,TRUE);
      if(!success)
         throw std::runtime_error("failed to install cancel handler");
   }
   else
   {
      gRefCnt--;
      ::SetConsoleCtrlHandler(&onHandleThunk,FALSE);
   }
}

void keyMonitor::waitForCancelUntil(HANDLE h, std::function<void(void)> f)
{
   HANDLE hans[2] = { h, m_evt.getHandle() };
   auto rVal = ::WaitForMultipleObjects(2,hans,/*all?*/FALSE,INFINITE);
   if(rVal == WAIT_OBJECT_0 + 1)
      f();
}

BOOL WINAPI keyMonitor::onHandleThunk(DWORD ctrl)
{
   if(!gHandler)
      return FALSE;

   if(ctrl == CTRL_C_EVENT || ctrl == CTRL_BREAK_EVENT || ctrl == CTRL_CLOSE_EVENT)
   {
      gHandler->onCancel();
      return TRUE;
   }
   else
      return FALSE;
}

tcatExposeSingletonTypeAs(keyMonitor,iKeyMonitor);

} // namespace cancel
