#ifndef ___exec_scriptRunner___
#define ___exec_scriptRunner___

#include "../cmn/win32.hpp"
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

class job : public iJob {
public:
   job();
   virtual void terminate();
   void attachProcess(HANDLE h);

private:
   cmn::autoHandle m_h;
};

class processRunner : public iProcessRunner {
public:
   virtual HANDLE execute(iJob *pJob, const char *command, iOutPipe *pStdOut = NULL, iOutPipe *pStdErr = NULL, std::function<void(DWORD)> onCreate = std::function<void(DWORD)>());
};

} // namespace exec

#endif // ___exec_scriptRunner___
