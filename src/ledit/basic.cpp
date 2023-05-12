#include "../cmn/service.hpp"
#include "../resolve/api.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"

namespace ledit {

class basicCmdLineKeyHandler : public iCmdLineKeyHandler {
public:
   basicCmdLineKeyHandler()
   : m_resolver(m_svcMan->demand<resolve::iProgramResolver>())
   , m_history(m_svcMan->demand<ledit::iCmdLineHistory>())
   {}

   virtual bool tryHandle(extKey c, cmdLineState& s)
   {
      if(c.is(27)) // escape
      {
         s.userText = "";
         s.iCursor = 0;
         reguess(s);
         s.resolved = m_resolver.tryResolve(s.userText);
         return true;
      }

      else if(c.modIs(75)) // left arrow
      {
         if(s.iCursor)
            s.iCursor--;
         return true;
      }
      else if(c.modIs(77)) // right arrow
      {
         if(((size_t)s.iCursor) < s.userText.length())
            s.iCursor++;
         return true;
      }

      else if(c.modIs(71)) // home
      {
         s.iCursor = 0;
         return true;
      }
      else if(c.modIs(79)) // end
      {
         s.iCursor = s.userText.length();
         return true;
      }

      else if(c.controlModIs(116)) // ctrl-right arrow
      {
         if(((size_t)s.iCursor) == s.userText.length() && s.guessSuffix.length())
         {
            s.userText += std::string(s.guessSuffix.c_str(),1);
            s.iCursor = s.userText.length();
            reguess(s);
         }
         return true;
      }
      else if(c.controlModIs(117)) // ctrl-end
      {
         if(s.guessSuffix.length())
         {
            s.userText += s.guessSuffix;
            s.iCursor = s.userText.length();
            reguess(s);
         }
         return true;
      }

      else if(c.is(8)) // backspace
      {
         if(s.iCursor)
         {
            std::string left(s.userText.c_str(),(s.iCursor-1));
            std::string right(s.userText.c_str()+s.iCursor);
            s.userText = (left + right);
            s.iCursor--;
            reguess(s);
            s.resolved = m_resolver.tryResolve(s.userText);
         }
         return true;
      }

      else if(c.modIs(72)) // up
      {
         s.userText = m_history.get(s.histIdx,/*up*/true);
         s.iCursor = s.userText.length();
         reguess(s);
         s.resolved = m_resolver.tryResolve(s.userText);
         return true;
      }
      else if(c.modIs(80)) // down
      {
         s.userText = m_history.get(s.histIdx,/*up*/false);
         s.iCursor = s.userText.length();
         reguess(s);
         s.resolved = m_resolver.tryResolve(s.userText);
         return true;
      }

      else if(c.is(13)) // enter
      {
         s.readyToSend = true;
         return true;
      }

      return false;
   }

   virtual bool tryHandleLast(extKey c, cmdLineState& s)
   {
      if(c.mod != 0)
         return false;
      if(!::isprint(c.base))
         return false;

      std::string left(s.userText.c_str(),s.iCursor);
      std::string right(s.userText.c_str()+s.iCursor);
      s.userText = (left + std::string(1,c.base) + right);
      s.iCursor++;
      reguess(s);
      s.resolved = m_resolver.tryResolve(s.userText);
      return true;
   }

private:
   void reguess(cmdLineState& s)
   {
      s.guessSuffix = "";
      if(s.userText.length() == 0)
         return;

      auto g = m_history.matchBest(s.userText);
      if(g.length())
         s.guessSuffix = g.c_str() + s.userText.length();
   }

   tcat::typePtr<cmn::serviceManager> m_svcMan;
   resolve::iProgramResolver& m_resolver;
   ledit::iCmdLineHistory& m_history;
};

tcatExposeTypeAs(basicCmdLineKeyHandler,iCmdLineKeyHandler);

} // namespace ledit
