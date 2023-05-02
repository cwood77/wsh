#include "../cmn/autoPtr.hpp"
#include "../exec/cancel.hpp"
#include "../tcatlib/api.hpp"
#include "scriptRunner.hpp"
#include <stdexcept>

namespace exec {

outPipe::outPipe()
{
   SECURITY_ATTRIBUTES saAttr;
   saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
   saAttr.bInheritHandle = TRUE;
   saAttr.lpSecurityDescriptor = NULL;

   if(!::CreatePipe(&m_parentEnd,&m_childEnd,&saAttr,0))
      throw std::runtime_error("::CreatePipe failed");

   if(!::SetHandleInformation(m_parentEnd,HANDLE_FLAG_INHERIT,0))
      throw std::runtime_error("::SetHandleInformation failed");
}

outPipe::~outPipe()
{
   closeChildEnd();
   ::CloseHandle(m_parentEnd);
}

void outPipe::processLoop(std::function<void(const std::string&)> f, size_t bufferSize)
{
   tcat::typePtr<cancel::iKeyMonitor> cancel;

   cmn::sizedAlloc block;
   block.realloc(bufferSize);

   for(;;)
   {
      DWORD dwRead = 0;
      BOOL bSuccess;
      {
         cancel::autoInstallSyncIo _io(*cancel,::GetCurrentThread());
         bSuccess = ::ReadFile(m_parentEnd,block.ptr(),block.size(),&dwRead,NULL);
      }
      if(!bSuccess || dwRead == 0) break;

      std::string s(block.ptr());
      f(s);
   }
}

tcatExposeTypeAs(outPipe,iOutPipe);

void processRunner::execute(HANDLE hJob, const char *command, iOutPipe *pStdOut, iOutPipe *pStdErr, std::function<void(DWORD)> onCreate)
{
   STARTUPINFOA si;
   ::memset(&si,0,sizeof(STARTUPINFOA));
   si.cb = sizeof(STARTUPINFOA);
   si.hStdError  = pStdErr ? pStdErr->getChildEnd() : GetStdHandle(STD_ERROR_HANDLE);
   si.hStdOutput = pStdOut ? pStdOut->getChildEnd() : GetStdHandle(STD_OUTPUT_HANDLE);
   si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
   if(pStdOut || pStdErr)
      si.dwFlags |= STARTF_USESTDHANDLES;

   PROCESS_INFORMATION pi;
   ::memset(&pi,0,sizeof(PROCESS_INFORMATION));

   std::string _command = command;

   BOOL success = ::CreateProcessA(
      NULL,                                // lpApplicationName,
      const_cast<char*>(_command.c_str()), // lpCommandLine,
      NULL,                                // lpProcessAttributes,
      NULL,                                // lpThreadAttributes,
      TRUE,                                // bInheritHandles,
      CREATE_SUSPENDED,                    // dwCreationFlags,
      NULL,                                // lpEnvironment,
      NULL,                                // lpCurrentDirectory,
      &si,                                 // lpStartupInfo,
      &pi                                  // lpProcessInformation
   );
   if(!success)
      throw std::runtime_error("failed to create process");

   onCreate(pi.dwProcessId);
   ::ResumeThread(pi.hThread);

   ::CloseHandle(pi.hProcess);
   ::CloseHandle(pi.hThread);

   if(pStdOut)
      pStdOut->closeChildEnd();
   if(pStdErr)
      pStdErr->closeChildEnd();
}

tcatExposeTypeAs(processRunner,iProcessRunner);

} // namespace exec

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
