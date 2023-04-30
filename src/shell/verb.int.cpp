#define WIN32_LEAN_AND_MEAN
#include "../cmn/autoPtr.hpp"
#include "../cmn/service.hpp"
#include "../console/arg.hpp"
#include "../console/log.hpp"
#include "../cui/api.hpp"
#include "../cui/pen.hpp"
#include "../file/api.hpp"
#include "../file/manager.hpp"
#include "../tcatlib/api.hpp"
#include <memory>

namespace {

class playCommand : public console::iCommand {
public:
   playCommand() : oNoAnimations(false), oNoRandom(false) {}

   std::string oServerAddr;
   std::string oAccount;
   std::string oPassword;

   std::string oCannedInputFile;
   bool        oNoAnimations;
   bool        oNoRandom;

   virtual void run(console::iLog& l);
};

class myVerb : public console::globalVerb {
protected:
   virtual console::verbBase *inflate()
   {
      std::unique_ptr<console::verbBase> v(
         new console::verb<playCommand>("--play"));

      v->addParameter(
         console::stringParameter::required(offsetof(playCommand,oServerAddr)));
      v->addParameter(
         console::stringParameter::required(offsetof(playCommand,oAccount)));
      v->addParameter(
         console::stringParameter::required(offsetof(playCommand,oPassword)));

      v->addParameter(
         console::stringParameter::optional(offsetof(playCommand,oCannedInputFile)));
      v->addOption(
         *new console::boolOption("--noAni",offsetof(playCommand,oNoAnimations)));
      v->addOption(
         *new console::boolOption("--noRnd",offsetof(playCommand,oNoRandom)));

      return v.release();
   }
} gVerb;

void playCommand::run(console::iLog& l)
{
   tcat::typePtr<file::iFileManager> fMan;
   l.writeLnDebug("loading config settings (optional)");
   cmn::autoReleasePtr<file::iSstFile> pFile(&fMan->bindFile<file::iSstFile>(
      file::iFileManager::kExeAdjacent,
      "config.sst",
      file::iFileManager::kReadOnly
   ));
   pFile->tie(l);

   l.writeLnDebug("compiling services");
   tcat::typePtr<cmn::serviceManager> svcMan;
   pen::object _pen(std::cout);
   cmn::autoService<pen::object> _penSvc(*svcMan,_pen);
   tcat::typePtr<cui::iUserInput> _in;
   tcat::typePtr<cui::iCannedUserInput> _in2;
   _in2->chain(*_in);
   cmn::autoService<cui::iUserInput> _inSvc(*svcMan,*_in2);

   l.writeLnDebug("loading canned input");
   if(!oCannedInputFile.empty())
   {
      cmn::autoReleasePtr<file::iSstFile> pFile(&fMan->bindFile<file::iSstFile>(
         file::iFileManager::kCurrentFolder,
         oCannedInputFile.c_str(),
         file::iFileManager::kReadOnly
      ));
      pFile->tie(l);
      if(!pFile->existed())
         throw std::runtime_error("couldn't find canned input file");

      std::vector<char> v;
      auto& arr = pFile->dict()["keys"].as<sst::array>();
      for(size_t i=0;i<arr.size();i++)
         v.push_back(arr[i].as<sst::str>().get().c_str()[0]);

      _in2->configure(v);
   }

   l.writeLnDebug("switching to cui");
   pen::object::setupStdOut();
   _pen.str() << pen::showCursor(false);

   // return to normalcy
   _pen.str() << pen::fgcol(pen::kDefault) << pen::bgcol(pen::kDefault) << pen::showCursor();
}

} // anonymous namespace