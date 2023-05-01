#include "../cmn/service.hpp"
#include "../cmn/win32.hpp"
#include "../cui/api.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"

namespace outcor {

class sink : public iSink {
public:
   sink(cmn::mutex& m, cui::iStyler& s, cui::iStylePrefs::styles st)
   : m_lock(m), m_styler(s), m_style(st) {}

   virtual void release() { delete this; }

   virtual void write(time_t ts, const std::string& s)
   {
      m_styler.with(m_style,[&](auto& o){ o << s; });
   }

private:
   cmn::autoLock m_lock;
   cui::iStyler& m_styler;
   cui::iStylePrefs::styles m_style;
};

class outCorrelator : public iOutCorrelator {
public:
   outCorrelator()
   : m_styler(m_svcMan->demand<cui::iStyler>())
   {
   }

   virtual iSink& lock(bool isOut)
   {
      return *new sink(m_mutex,m_styler,
         isOut ? cui::iStylePrefs::kNormal : cui::iStylePrefs::kError);
   }

private:
   tcat::typePtr<cmn::serviceManager> m_svcMan;
   cui::iStyler& m_styler;
   cmn::mutex m_mutex;
};

tcatExposeTypeAs(outCorrelator,iOutCorrelator);

} // namespace outcor

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
