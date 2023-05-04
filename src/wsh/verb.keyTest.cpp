#define WIN32_LEAN_AND_MEAN
#include "../cmn/autoPtr.hpp"
#include "../cmn/service.hpp"
#include "../cmn/win32.hpp"
#include "../cmn/wshsubproc.hpp"
#include "../console/arg.hpp"
#include "../console/log.hpp"
#include "../cui/api.hpp"
#include "../cui/pen.hpp"
#include "../exec/cancel.hpp"
#include "../file/api.hpp"
#include "../file/manager.hpp"
#include "../outcor/api.hpp"
#include "../resolve/api.hpp"
#include "../tcatlib/api.hpp"
#include "runLoop.hpp"
#include <memory>

namespace {

class keyTestCommand : public console::iCommand {
public:
   std::string oCannedInputFile;

   virtual void run(console::iLog& l);
};

class myVerb : public console::globalVerb {
protected:
   virtual console::verbBase *inflate()
   {
      std::unique_ptr<console::verbBase> v(
         new console::verb<keyTestCommand>("--keyTest"));

      v->addParameter(
         console::stringParameter::optional(offsetof(keyTestCommand,oCannedInputFile)));

      return v.release();
   }
} gVerb;

void keyTestCommand::run(console::iLog& l)
{
   l.writeLnInfo("escape to quit");

   tcat::typePtr<cui::iUserInput> in;
   char c = 0;
   while(c != 27)
   {
      c = in->getKey();
      l.writeLnInfo("<%d>",(int)c);
   }
}

} // anonymous namespace

