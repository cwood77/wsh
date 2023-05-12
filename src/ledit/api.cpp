#define WIN32_LEAN_AND_MEAN
#include "../cmn/autoPtr.hpp"
#include "../cmn/service.hpp"
#include "../cui/pen.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include "printer.hpp"
#include <windows.h>

namespace ledit {
   
class cmdLineEditor : public iCmdLineEditor {
public:
   virtual cmdLineResult run();
};

cmdLineResult cmdLineEditor::run()
{
   tcat::typePtr<cmn::serviceManager> svcMan;
   tcat::typeSet<iCmdLineKeyHandler> handlers;
   tcat::typePtr<iCmdHelp> helper;
   auto& in = svcMan->demand<cui::iUserInput>();
   auto& out = svcMan->demand<pen::object>();
   auto& styler = svcMan->demand<cui::iStyler>();

   do
   {
      cmdLineState state;
      printer(out,styler).initialize(state);
      while(!state.readyToSend)
      {
         printer(out,styler).print(state);

         extKey c;
         c.base = in.getKey();
         if(c.base == -32)
         {
            auto ctl = in.hasControl();
            c.flags = (ctl ? 2 : 0);
            c.mod = c.base;
            c.base = in.getKey();
         }
         else if(c.base == 9)
         {
            auto s = in.hasShift();
            c.mod = (s ? 1 : 0);
         }

         bool handled = false;
         for(size_t i=0;!handled&&i<handlers.size();i++)
            handled = handlers[i]->tryHandle(c,state);
         for(size_t i=0;!handled&&i<handlers.size();i++)
            handled = handlers[i]->tryHandleLast(c,state);

         auto newHelp = helper->getHelp(state);
         printer(out,styler).updateHelp(state,newHelp);

         /*if(!handled)
         {
            out.str() << std::endl;
            styler.error([](auto& o){ o << "no bueno" << std::endl; });
            break;
         }
         else*/ if(state.readyToSend)
         {
            cmdLineResult result;
            result.userText = state.userText;
            result.resolvedCommand =
               state.resolved.empty() ? state.userText : state.resolved;
            return result;
         }
      }

   } while(true);
}

tcatExposeTypeAs(cmdLineEditor,iCmdLineEditor);

} // namespace ledit

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
