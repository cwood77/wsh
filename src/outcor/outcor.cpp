#include "../cmn/service.hpp"
#include "../cmn/win32.hpp"
#include "../cui/pen.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"

namespace outcor {

class sink : public iSink {
public:
   sink(cmn::mutex& m, pen::object& p)
   : m_lock(m), m_pen(p) {}

   virtual void release() { delete this; }

   virtual void write(time_t ts, const std::string& s)
   {
      m_pen.str()
         // return colors to a standard baseline
         << pen::fgcol(pen::kDefault) << pen::bgcol(pen::kDefault)
         << s;
   }

private:
   cmn::autoLock m_lock;
   pen::object& m_pen;
};

class outCorrelator : public iOutCorrelator {
public:
   outCorrelator()
   : m_pen(m_svcMan->demand<pen::object>())
   {
   }

   virtual iSink& lock(bool /*isOut*/)
   {
      return *new sink(m_mutex,m_pen);
   }

private:
   tcat::typePtr<cmn::serviceManager> m_svcMan;
   pen::object& m_pen;
   cmn::mutex m_mutex;
};

tcatExposeTypeAs(outCorrelator,iOutCorrelator);

} // namespace outcor

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
