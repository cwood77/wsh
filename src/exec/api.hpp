#ifndef ___exec_api___
#define ___exec_api___

#include "../cmn/win32.hpp"
#include <functional>

namespace exec {

class iOutPipe {
public:
   virtual ~iOutPipe() {}

   virtual HANDLE getChildEnd() = 0;
   virtual void closeChildEnd() = 0;

   virtual void processLoop(
      std::function<void(const std::string&)> f,
      size_t bufferSize = 4096) = 0;
};

class iJob {
public:
   virtual ~iJob() {}

   virtual void terminate() = 0;
};

class iProcessRunner {
public:
   virtual ~iProcessRunner() {}

   virtual void execute(iJob *pJob, const char *command,
      iOutPipe *pStdOut = NULL,
      iOutPipe *pStdErr = NULL,
      std::function<void(DWORD)> onCreate = std::function<void(DWORD)>()) = 0;
};

} // namespace exec

#endif // ___exec_api___
