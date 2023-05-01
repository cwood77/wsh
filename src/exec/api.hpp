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

class iProcessRunner {
public:
   virtual ~iProcessRunner() {}

   virtual void execute(HANDLE hJob, const char *command,
      iOutPipe *pStdOut = NULL,
      iOutPipe *pStdErr = NULL) = 0;
};

#if 0
class pipeReadingThread : public cmn::iThread {
};

class iOutPipeSet {
public:
   virtual iOutPipe& out() = 0;
   virtual iOutPipe& err() = 0;
};
#endif

} // namespace exec

#endif // ___exec_api___
