#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <cstring>

namespace ledit {

class cmdHelp : public iCmdHelp {
public:
   virtual std::string getHelp(cmdLineState& s)
   {
      if(s.readyToSend)
         return "";

      if(::strncmp(s.userText.c_str(),"tree",4)==0)
         return "<path> [/F]";

      else if(s.userText == "cmd /")
         return "[C termintate] [K remain]";
      else if(::strncmp(s.userText.c_str(),"cmd",3)==0)
         return "[/CK] <expr>";

      return "";
   }
};

tcatExposeTypeAs(cmdHelp,iCmdHelp);

} // namespace ledit
