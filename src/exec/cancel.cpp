#include "cancel.hpp"
#include "../tcatlib/api.hpp"

namespace cancel {

class keyMonitor : public iKeyMonitor {
public:
   keyMonitor();
   ~keyMonitor();
   virtual void install(bool add);
   virtual void recordThreadIo(HANDLE h, bool add);

   virtual bool wasAborted() const { return m_wasAborted; }

private:
   void onCancel();

   bool m_wasAborted;

   static BOOL WINAPI onHandleThunk(DWORD ctrl);
   static int gRefCnt;
   static keyMonitor *gHandler;
};

int keyMonitor::gRefCnt = 0;
keyMonitor *keyMonitor::gHandler = NULL;

keyMonitor::keyMonitor()
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

void keyMonitor::recordThreadIo(HANDLE h, bool add)
{
}

void keyMonitor::onCancel()
{
   m_wasAborted = true;
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
