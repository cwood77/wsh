#define WIN32_LEAN_AND_MEAN
#include "../cmn/autoPtr.hpp"
#include "../cmn/service.hpp"
#include "../console/arg.hpp"
#include "../console/log.hpp"
#include "../cui/api.hpp"
#include "../cui/pen.hpp"
#include "../file/api.hpp"
#include "../file/manager.hpp"
#include "../ledit/api.hpp"
#include "../tcatlib/api.hpp"
#include <memory>

namespace {

class intCommand : public console::iCommand {
public:
   std::string oCannedInputFile;

   virtual void run(console::iLog& l);
};

class myVerb : public console::globalVerb {
protected:
   virtual console::verbBase *inflate()
   {
      std::unique_ptr<console::verbBase> v(
         new console::verb<intCommand>("--int"));

      v->addParameter(
         console::stringParameter::optional(offsetof(intCommand,oCannedInputFile)));

      return v.release();
   }
} gVerb;

void intCommand::run(console::iLog& l)
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
   tcat::typePtr<cui::iStylePrefs> _stylePrefs;
   tcat::typePtr<cui::iStyler> _styler;
   _styler->bind(*_stylePrefs,_pen.str());
   cmn::autoService<cui::iStyler> _stylerSvc(*svcMan,*_styler);

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
   //_pen.str() << pen::showCursor(false);

   //_pen.str() << pen::fgcol(pen::kYellow) << "\xea:" << pen::fgcol(pen::kDefault) << std::endl;

   // listen for chars
   //
   // line editing
   // - esc
   //   clear line
   // - bs
   //   remove char
   // - left arrow/right arrow/home/end
   //   move cursor
   // - other
   //   append character
   //   display guess text if this matches history
   //
   // history
   // - up arrow/down arrow
   //   navigate history
   //
   // completion
   // - tab/shift-tab
   //   take entered chars and search -or- cycle through searches
   // - space
   //   decide if this is a command, and if so, pull up help
   //
   // guess
   // - ctrl-tab
   //   take a word of the guess text
   // - ctrl-end
   //   take all guess text and jump to end
   //
   // exec
   // - enter
   //   break send command
   //

   tcat::typePtr<ledit::iCmdLineEditor> editor;
   editor->run();

   // return to normalcy
   _pen.str() << pen::fgcol(pen::kDefault) << pen::bgcol(pen::kDefault);// << pen::showCursor();
}

} // anonymous namespace
