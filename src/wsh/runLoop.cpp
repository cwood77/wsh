#include "../cmn/wshsubproc.hpp"
#include "runLoop.hpp"

namespace wsh {

runLoop::runLoop()
: m_styler(m_svcMan->demand<cui::iStyler>())
, m_out(m_svcMan->demand<outcor::iOutCorrelator>())
{
}

void runLoop::start()
{
   while(true)
   {
      try
      {
         runOnce();
      }
      catch(cmn::wshQuitException& x)
      {
         return;
      }
      catch(std::exception& x)
      {
         m_styler.error([&](auto& o)
            { o << std::endl << "ERROR: " << x.what() << std::endl; });
      }
   }
}

void runLoop::runOnce()
{
   auto wholecmd = m_editor->run();

   tcat::typePtr<ledit::iCmdLineResultSplitter> splitter;
   auto v = splitter->split(wholecmd);

   bool success = true;
   for(auto cmd : v)
   {
      m_styler.hint([&](auto& o)
         { o << std::endl << "[running '" << cmd.resolvedCommand << "']" << std::endl; });

      m_sub->beginExecute(m_out,cmd);
      success = m_sub->join();

      m_styler.hint([&](auto& o)
         { o << "[done: '" << cmd.resolvedCommand  << "']" << std::endl; });

      if(!success)
         break;
   }

   if(success)
   {
      // record only successful command
      auto& hist = m_svcMan->demand<ledit::iCmdLineHistory>();
      hist.add(wholecmd.userText);
   }
}

} // namespace wsh
