#ifndef ___exec_scriptRunner___
#define ___exec_scriptRunner___

#include "api.hpp"

namespace exec {

class outPipe : public iOutPipe {
public:
   outPipe();
   ~outPipe();

   virtual HANDLE getChildEnd() { return m_childEnd; }
   virtual void closeChildEnd()
   { ::CloseHandle(m_childEnd); m_childEnd = INVALID_HANDLE_VALUE; }

   virtual void processLoop(
      std::function<void(const std::string&)> f,
      size_t bufferSize = 4096);

private:
   HANDLE m_parentEnd;
   HANDLE m_childEnd;
};

class processRunner : public iProcessRunner {
public:
   virtual void execute(HANDLE hJob, const char *command, iOutPipe *pStdOut = NULL, iOutPipe *pStdErr = NULL);
};

} // namespace exec

#endif // ___exec_scriptRunner___
