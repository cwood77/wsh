#define WIN32_LEAN_AND_MEAN
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <windows.h>

namespace resolve {

class programResolver : public iProgramResolver {
public:
   virtual std::string tryResolve(const std::string& hintPath)
   {
      if(hintPath == "q")
         return "bin\\out\\debug\\quit.exe";
      else
         return "";
   }
};

tcatExposeTypeAs(programResolver,iProgramResolver);

} // namespace resolve

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
