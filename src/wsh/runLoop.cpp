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
   auto cmd = m_editor->run();

   m_styler.hint([&](auto& o)
      { o << std::endl << "[running '" << cmd.resolvedCommand << "']" << std::endl; });

   m_sub->beginExecute(m_out,cmd);
   m_sub->join();

   m_styler.hint([&](auto& o)
      { o << "[done: '" << cmd.resolvedCommand  << "']" << std::endl; });
}

} // namespace wsh
