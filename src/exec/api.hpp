#ifndef ___exec_api___
#define ___exec_api___

#define WIN32_LEAN_AND_MEAN
#include "windows.h"

namespace exec {

class iProcessRunner {
public:
   virtual ~iProcessRunner() {}

   virtual void execute(HANDLE hJob, const char *command) = 0;
};

} // namespace exec

#endif // ___exec_api___
