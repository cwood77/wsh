#include "../tcatlib/api.hpp"
#include "scriptRunner.hpp"
#include <stdexcept>

namespace exec {

void processRunner::execute(HANDLE hJob, const char *command)
{
   STARTUPINFOA si;
   ::memset(&si,0,sizeof(STARTUPINFOA));
   si.cb = sizeof(STARTUPINFOA);

   PROCESS_INFORMATION pi;
   ::memset(&pi,0,sizeof(PROCESS_INFORMATION));

   std::string _command = command;

   BOOL success = ::CreateProcessA(
      NULL,                                // lpApplicationName,
      const_cast<char*>(_command.c_str()), // lpCommandLine,
      NULL,                                // lpProcessAttributes,
      NULL,                                // lpThreadAttributes,
      FALSE,                               // bInheritHandles,
      CREATE_NO_WINDOW,                    // dwCreationFlags,
      NULL,                                // lpEnvironment,
      NULL,                                // lpCurrentDirectory,
      &si,                                 // lpStartupInfo,
      &pi                                  // lpProcessInformation
   );
   if(!success)
      throw std::runtime_error("failed to create process");

   ::CloseHandle(pi.hProcess);
   ::CloseHandle(pi.hThread);
}

tcatExposeTypeAs(processRunner,iProcessRunner);

} // namespace exec

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
