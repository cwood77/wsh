#include "runLoop.hpp"

namespace wsh {

runLoop::runLoop()
: m_styler(m_svcMan->demand<cui::iStyler>())
, m_out(m_svcMan->demand<outcor::iOutCorrelator>())
{
}

void runLoop::start()
{
   // TODO once only for now
   runOnce();
}

void runLoop::runOnce()
{
   auto cmd = m_editor->run();

   m_styler.hint([&](auto& o)
   { o << std::endl << "[running '" << cmd << "']" << std::endl; });

   m_sub->beginExecute(m_out,cmd);
   m_sub->join();

   m_styler.hint([&](auto& o)
   { o << "[done: '" << cmd << "']" << std::endl; });
}

} // namespace wsh
