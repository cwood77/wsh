#define WIN32_LEAN_AND_MEAN
#include "../cui/pen.hpp"
#include "api.hpp"
#include "printer.hpp"
#include <windows.h>

namespace ledit {

void printer::initialize(cmdLineState& s)
{
   m_style.normal([](auto& o){ o << std::endl; });
   {
      char path[MAX_PATH];
      ::GetCurrentDirectory(MAX_PATH,path);
      s.pwd = path;
   }
   m_style.pwd([&](auto& o){ o << s.pwd << std::endl; });

   // loc
   m_style.normal([&](auto& o){ o << pen::getPos(s.loc); });

   s.prompt = "\xea ";
   m_style.prompt([&](auto& o){ o << s.prompt; });
}

void printer::print(cmdLineState& s)
{
   cui::pnt xy = s.loc;
   xy.x += s.prompt.length();

   // don't both redrawing the prompt since it's immutable

   // usertext portion
   m_pen.str() << pen::moveTo(xy);
   m_style.normal([&](auto& o){ o << s.userText; });
   //xy.x += s.userText.length();

   // guess portion
   m_style.hint([&](auto& o){ o << s.guessSuffix; });

   // erase old stuff (if necessary)
   size_t totalSize = s.userText.length() + s.guessSuffix.length();
   if(s.lastNonPromptLength > totalSize)
   //{
      m_style.normal([&](auto& o){ o << std::string(s.lastNonPromptLength-totalSize,' '); });
      //m_pen.str() << pen::moveTo(xy);
   //}
   s.lastNonPromptLength = totalSize;

   // position the cursor where the user left it
   xy = s.loc;
   xy.x += s.prompt.length();
   xy.x += s.iCursor;
   m_pen.str() << pen::moveTo(xy);
}

void printer::updateHelp(cmdLineState& s, const std::string& newHelp)
{
   if(s.helpText == newHelp)
      return;

   // draw the new help text
   cui::pnt xy = s.loc;
   xy.y-=2;
   m_pen.str() << pen::moveTo(xy);
   m_style.help([&](auto& o){ o << newHelp; });

   // clear any leftover exposed from last time
   size_t totalSize = newHelp.length();
   if(s.helpText.length() > totalSize)
      m_style.normal([&](auto& o){ o << std::string(s.helpText.length()-totalSize,' '); });
   s.helpText = newHelp;

   // position the cursor where the user left it
   xy = s.loc;
   xy.x += s.prompt.length();
   xy.x += s.iCursor;
   m_pen.str() << pen::moveTo(xy);
}

} // namespace ledit
