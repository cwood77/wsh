#define WIN32_LEAN_AND_MEAN
#include "../tcatlib/api.hpp"
#include <windows.h>

#ifdef cdwTest
#include "../cmn/service.hpp"
#include "../cmn/win32.hpp"
#include "../cmn/wshsubproc.hpp"
#include "../cui/api.hpp"
#include "../cui/pen.hpp"
#include "../exec/cancel.hpp"
#include "../test/api.hpp"
#include "api.hpp"
#include <psapi.h>

namespace {

class processSnapshot {
public:
   processSnapshot() { recordProcesses(); }

   size_t count(const std::string& imageName);

private:
   void recordProcesses();
   void recordProcess(DWORD pid);

   std::list<std::string> m_processes;
};

void processSnapshot::recordProcesses()
{
   DWORD aProcesses[1024], cbNeeded;

   if(!EnumProcesses(aProcesses,sizeof(aProcesses),&cbNeeded))
      throw std::runtime_error("ise");

   // Calculate how many process identifiers were returned.
   const DWORD cProcesses = cbNeeded / sizeof(DWORD);

   // Print the name and process identifier for each process.
   for(unsigned int i=0;i<cProcesses;i++)
      if(aProcesses[i] != 0)
         recordProcess(aProcesses[i]);
}

void processSnapshot::recordProcess(DWORD pid)
{
   // Get a handle to the process.
   cmn::autoHandle hProc;
   hProc.h = ::OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,pid);

   // Get the process name.
   if(!hProc.h)
      return;

   HMODULE hMod;
   DWORD cbNeeded;
   if(!::EnumProcessModules(hProc.h,&hMod,sizeof(hMod),&cbNeeded))
      return;

   char szProcessName[MAX_PATH];
   ::GetModuleBaseNameA(hProc.h,hMod,szProcessName,sizeof(szProcessName));
   m_processes.push_back(szProcessName);
}

size_t processSnapshot::count(const std::string& imageName)
{
   size_t i = 0;

   for(auto& x : m_processes)
      if(x == imageName)
         i++;

   return i;
}

class fakeKeyMonitor : public cancel::iKeyMonitor {
public:
   virtual void install(bool add) {}

   virtual void waitForCancelUntil(HANDLE h, std::function<void(void)> f)
   {
      if(m_wasAborted)
         f();
   }

   virtual void clearAborted() { m_wasAborted = false; }

   bool m_wasAborted;
};

static void testSubprocessFacade(std::function<void(outcor::iOutCorrelator&,outcor::iSubprocessFacade&)> f)
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
   fakeKeyMonitor _cancel;
   _cancel.m_wasAborted = false;
   cmn::autoService<cancel::iKeyMonitor> _cancelSvc(*svcMan,_cancel);

   // test body
   tcat::typePtr<outcor::iOutCorrelator> out;
   tcat::typePtr<outcor::iSubprocessFacade> sub;
   f(*out,*sub);
}

testDefineTest(outcor_shmemRoundtripTailCmdWorks)
{
   testSubprocessFacade([&](auto& out, auto& sub)
   {
      sub.beginExecute(out,ledit::cmdLineResult("q","bin\\out\\debug\\q.exe"));
      try
      {
         sub.join();
         a.assertTrue(false);
      }
      catch(cmn::wshQuitException& x)
      {
         a.assertTrue(true);
      }
   });
}

testDefineTest(outcor_cancelKillsJob)
{
   testSubprocessFacade([&](auto& out, auto& sub)
   {
      // create a childprocess signal
      cmn::osEvent evt("cdw_outcor_cancelJobKill_evt");
      a.assertTrue(!evt.isSet());

      // start the child process and wait for it to hang
      sub.beginExecute(
         out,ledit::cmdLineResult(
            "<test>",
            "bin\\out\\debug\\childTest.exe --recurse 30 --set cdw_outcor_cancelJobKill_evt --hang"));
      evt.waitAndThrowIfTimeout(3*1000);

      // make sure the child is running as expected
      {
         processSnapshot snap;
         size_t cnt = snap.count("childTest.exe");
         a.assertTrue(30+1 == cnt);
      }

      // simulate a Ctrl-C
      tcat::typePtr<cmn::serviceManager> svcMan;
      auto& cancel = svcMan->demand<cancel::iKeyMonitor>();
      dynamic_cast<fakeKeyMonitor&>(cancel).m_wasAborted = true;
      sub.join();

      // make sure the child process is killed
      {
         processSnapshot snap;
         size_t cnt = snap.count("childTest.exe");
         a.assertTrue(0 == cnt);
      }

   });
}

} // anonymous namespace

#endif // cdwTest

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
