#ifndef ___exec_scriptRunner___
#define ___exec_scriptRunner___

#include "api.hpp"

namespace exec {

class processRunner : public iProcessRunner {
public:
   virtual void execute(HANDLE hJob, const char *command);
};

} // namespace exec

#endif // ___exec_scriptRunner___
