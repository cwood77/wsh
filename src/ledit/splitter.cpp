#include "../cmn/service.hpp"
#include "../resolve/api.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <cstring>
#include <vector>

namespace ledit {

class cmdLineResultSplitter : public iCmdLineResultSplitter {
public:
   virtual std::vector<cmdLineResult> split(cmdLineResult& r)
   {
      const char *pThumb = ::strstr(r.userText.c_str(),"&&");
      if(!pThumb)
      {
         std::vector<cmdLineResult> rval;
         rval.push_back(r);
         return rval;
      }

      // split r into lhs and rhs
      cmdLineResult lhs;
      lhs.userText = std::string(r.userText.c_str(),pThumb-r.userText.c_str());

      pThumb+=2;
      for(;*pThumb==' ';++pThumb);

      cmdLineResult rhs;
      rhs.userText = pThumb;

      // resolve lhs and rhs
      tcat::typePtr<cmn::serviceManager> svcMan;
      auto& resolver = svcMan->demand<resolve::iProgramResolver>();
      lhs.resolvedCommand = resolver.tryResolve(lhs.userText);
      if(lhs.resolvedCommand.empty()) lhs.resolvedCommand = lhs.userText;
      rhs.resolvedCommand = resolver.tryResolve(rhs.userText);
      if(rhs.resolvedCommand.empty()) rhs.resolvedCommand = rhs.userText;

      // recurse
      auto rval = split(rhs);
      rval.insert(rval.begin(),lhs);
      return rval;
   }
};

tcatExposeTypeAs(cmdLineResultSplitter,iCmdLineResultSplitter);

} // namespace ledit
