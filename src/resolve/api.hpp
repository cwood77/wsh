#ifndef ___resolve_api___
#define ___resolve_api___

#include <string>

namespace resolve {

class iProgramResolver {
public:
   virtual ~iProgramResolver() {}

   virtual void addBuiltIn(const std::string& cmd) = 0;
   virtual void adoptFromCmdExe(const std::string& cmd) = 0;

   virtual std::string tryResolve(const std::string& hintPath) = 0;
};

} // namespace resolve

#endif // ___resolve_api___
