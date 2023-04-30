#define WIN32_LEAN_AND_MEAN
#include "../cui/pen.hpp"
#include "api.hpp"
#include "printer.hpp"
#include <windows.h>

namespace ledit {

void printer::initialize(cmdLineState& s)
{
   // loc
   {
      auto h = ::GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO bInfo;
      ::GetConsoleScreenBufferInfo(h,&bInfo);
      s.loc.x = bInfo.dwCursorPosition.X + 1;
      s.loc.y = bInfo.dwCursorPosition.Y + 1;
   }

   s.prompt = "\xea ";

   m_style.prompt([&](auto& o){ o << s.prompt; });
}

void printer::print(cmdLineState& s)
{
   cui::pnt xy = s.loc;
   xy.x += s.prompt.length();

   // usertext portion
   m_pen.str() << pen::moveTo(xy);
   m_style.normal([&](auto& o){ o << s.userText; });
   xy.x += s.userText.length();

   // erase old stuff (if necessary)
   size_t totalSize = s.userText.length();
   if(s.lastNonPromptLength > totalSize)
   {
      m_style.normal([&](auto& o){ o << std::string(s.lastNonPromptLength-totalSize,' '); });
      m_pen.str() << pen::moveTo(xy);
   }
}

} // namespace ledit
