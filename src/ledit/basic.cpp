#include "api.hpp"
#include "../tcatlib/api.hpp"

namespace ledit {

class basicCmdLineKeyHandler : public iCmdLineKeyHandler {
public:
   virtual bool tryHandle(char c, cmdLineState& s)
   {
      // eventually, handle
      // - escape
      // - enter
      // - left,right
      // - home,end
      // - backspace

      if(c == 27)
      {
         throw std::runtime_error("temporary quit via exception on escape");
      }
      else if(c == 13)
      {
         s.readyToSend = true;
         return true;
      }
      return false;
   }

   virtual bool tryHandleLast(char c, cmdLineState& s)
   {
      // TODO needs to account for cursor position!
      s.userText += std::string(1,c);
      return true;
   }
};

tcatExposeTypeAs(basicCmdLineKeyHandler,iCmdLineKeyHandler);

} // namespace ledit
