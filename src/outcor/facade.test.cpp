#define WIN32_LEAN_AND_MEAN
#include "../tcatlib/api.hpp"
#include <windows.h>

#ifdef cdwTest
#include "../cmn/service.hpp"
#include "../cmn/win32.hpp"
#include "../cmn/wshsubproc.hpp"
#include "../cui/api.hpp"
#include "../cui/pen.hpp"
#include "../test/api.hpp"
#include "api.hpp"

namespace {

testDefineTest(outcor_shmemRoundtripTailCmdWorks)
{
   // shmem
   cmn::shmem<cmn::wshMasterBlock> shmemMaster(
      cmn::buildWshMasterShmemName(::GetCurrentProcessId()));
   if(shmemMaster.existed())
      throw std::runtime_error("shmem already exists?");
   shmemMaster->version = cmn::wshMasterBlock::kCurrentVersion;

   // services
   std::stringstream actualOut;
   tcat::typePtr<cmn::serviceManager> svcMan;
   pen::object _pen(actualOut);
   cmn::autoService<pen::object> _penSvc(*svcMan,_pen);
   tcat::typePtr<cui::iStylePrefs> _stylePrefs;
   tcat::typePtr<cui::iStyler> _styler;
   _styler->bind(*_stylePrefs,_pen.str());
   cmn::autoService<cui::iStyler> _stylerSvc(*svcMan,*_styler);
   cmn::autoService<cmn::wshMasterBlock> _shmemMasterSvc(*svcMan,*shmemMaster);

   // test body
   tcat::typePtr<outcor::iOutCorrelator> out;
   tcat::typePtr<outcor::iSubprocessFacade> sub;
   sub->beginExecute(*out,"q");
   try
   {
      sub->join();
      a.assertTrue(false);
   }
   catch(cmn::wshQuitException& x)
   {
      a.assertTrue(true);
   }
}

} // anonymous namespace

#endif // cdwTest

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
