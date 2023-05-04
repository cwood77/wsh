#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <cstring>
#include <map>

namespace ledit {

class cmdHelp : public iCmdHelp {
public:
   virtual void addStartsWithHelp(const std::string& match, const std::string& help)
   {
      m_startsWith[match] = help;
   }

   virtual void addExactHelp(const std::string& match, const std::string& help)
   {
      m_exact[match] = help;
   }

   virtual std::string getHelp(cmdLineState& s)
   {
      if(s.readyToSend)
         return "";

      auto it = m_exact.find(s.userText);
      if(it != m_exact.end())
         return it->second;

      const char *pThumb = s.userText.c_str();
      for(;*pThumb&&*pThumb!=' ';++pThumb);
      std::string firstPart(s.userText.c_str(),pThumb-s.userText.c_str());

      it = m_startsWith.find(firstPart);
      if(it != m_startsWith.end())
         return it->second;

      return "";
   }

private:
   std::map<std::string,std::string> m_startsWith;
   std::map<std::string,std::string> m_exact;
};

tcatExposeSingletonTypeAs(cmdHelp,iCmdHelp);

} // namespace ledit
