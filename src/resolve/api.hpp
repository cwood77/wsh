#ifndef ___resolve_api___
#define ___resolve_api___

#include <string>

namespace resolve {

class iProgramResolver {
public:
   virtual ~iProgramResolver() {}
   virtual std::string tryResolve(const std::string& hintPath) = 0;
};

} // namespace resolve

#endif // ___resolve_api___
