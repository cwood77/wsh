#define WIN32_LEAN_AND_MEAN
#include "../file/manager.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <map>
#include <windows.h>

namespace resolve {

class programResolver : public iProgramResolver {
public:
   programResolver()
   {
      tcat::typePtr<file::iFileManager> fMan;
      std::string realPath = fMan->calculatePath(
         file::iFileManager::kExeAdjacent,
         "fork.exe \"gvim .\"");
      m_map["gvim"] = realPath;
   }

   virtual void addBuiltIn(const std::string& cmd)
   {
      tcat::typePtr<file::iFileManager> fMan;
      std::string realPath = fMan->calculatePath(
         file::iFileManager::kExeAdjacent,
         (cmd + ".exe").c_str());
      m_map[cmd] = realPath;
   }

   virtual void adoptFromCmdExe(const std::string& cmd)
   {
      m_map[cmd] = std::string("cmd.exe /C ") + cmd;
   }

   virtual std::string tryResolve(const std::string& hintPath)
   {
      const char *pThumb = hintPath.c_str();
      for(;*pThumb&&*pThumb!=' ';++pThumb);

      std::string word = hintPath;
      std::string rest;
      if(*pThumb==' ')
      {
         word = std::string(hintPath.c_str(),pThumb-hintPath.c_str());
         rest = pThumb;
      }

      auto it = m_map.find(word);
      if(it == m_map.end())
         return "";
      else
         return it->second + rest;
   }

private:
   std::map<std::string,std::string> m_map;
};

tcatExposeTypeAs(programResolver,iProgramResolver);

} // namespace resolve

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
