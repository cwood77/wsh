#define WIN32_LEAN_AND_MEAN
#include "../cmn/service.hpp"
#include "../cui/pen.hpp"
#include "../exec/cancel.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include "printer.hpp"
#include <windows.h>

namespace ledit {
   
class cmdLineEditor : public iCmdLineEditor {
public:
   virtual void run();
};

void cmdLineEditor::run()
{
   tcat::typePtr<cancel::iKeyMonitor> cancel;
   tcat::typePtr<cmn::serviceManager> svcMan;
   tcat::typeSet<iCmdLineKeyHandler> handlers;
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

         char c = 0;
         {
            cancel::autoInstallKeyMonitor _k(*cancel);
            c = in.getKey();
            if(cancel->wasAborted())
               styler.normal([](auto& o){ o << "was cancelled!" << std::endl; });
         }

         bool handled = false;
         for(size_t i=0;!handled&&i<handlers.size();i++)
            handled = handlers[i]->tryHandle(c,state);
         for(size_t i=0;!handled&&i<handlers.size();i++)
            handled = handlers[i]->tryHandleLast(c,state);

         if(!handled)
         {
            out.str() << std::endl;
            styler.error([](auto& o){ o << "no bueno" << std::endl; });
            break;
         }
         else if(state.readyToSend)
         {
            // send it
            return;
         }
      }

   } while(true);
}

tcatExposeTypeAs(cmdLineEditor,iCmdLineEditor);

} // namespace ledit

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
