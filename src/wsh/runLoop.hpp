#ifndef ___wsh_runLoop___
#define ___wsh_runLoop___

#include "../cmn/service.hpp"
#include "../ledit/api.hpp"
#include "../outcor/api.hpp"
#include "../tcatlib/api.hpp"

namespace wsh {

class runLoop {
public:
   runLoop();

   void start();

private:
   void runOnce();

   tcat::typePtr<cmn::serviceManager> m_svcMan;
   tcat::typePtr<ledit::iCmdLineEditor> m_editor;
   tcat::typePtr<outcor::iSubprocessFacade> m_sub;
   cui::iStyler& m_styler;
   outcor::iOutCorrelator& m_out;
};

} // namespace wsh

#endif // ___wsh_runLoop___
