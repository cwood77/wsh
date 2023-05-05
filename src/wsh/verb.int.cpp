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
#include "../ledit/api.hpp"
#include "../outcor/api.hpp"
#include "../resolve/api.hpp"
#include "../tcatlib/api.hpp"
#include "runLoop.hpp"
#include <fstream>
#include <memory>

namespace {

class intCommand : public console::iCommand {
public:
   std::string oCannedInputFile;

   virtual void run(console::iLog& l);

private:
   void setupDefaults(resolve::iProgramResolver& r);
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

   l.writeLnDebug("setup the shmem");
   cmn::shmem<cmn::wshMasterBlock> shmemMaster(
      cmn::buildWshMasterShmemName(::GetCurrentProcessId()));
   if(shmemMaster.existed())
      throw std::runtime_error("shmem already exists?");
   shmemMaster->version = cmn::wshMasterBlock::kCurrentVersion;

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
   tcat::typePtr<outcor::iOutCorrelator> _out;
   cmn::autoService<outcor::iOutCorrelator> _outSvc(*svcMan,*_out);
   tcat::typePtr<resolve::iProgramResolver> _res;
   setupDefaults(*_res);
   cmn::autoService<resolve::iProgramResolver> _resSvc(*svcMan,*_res);
   cmn::autoService<cmn::wshMasterBlock> _shmemMasterSvc(*svcMan,*shmemMaster);
   tcat::typePtr<cancel::iKeyMonitor> _cancel;
   cmn::autoService<cancel::iKeyMonitor> _cancelSvc(*svcMan,*_cancel);
   tcat::typePtr<ledit::iCmdLineHistory> _history;
   {
      std::string path = fMan->calculatePath(file::iFileManager::kExeAdjacent,"hist.txt");
      std::ifstream in(path.c_str());
      _history->load(in);
   }
   cmn::autoService<ledit::iCmdLineHistory> _historySvc(*svcMan,*_history);

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

   wsh::runLoop().start();

   {
      std::string path = fMan->calculatePath(file::iFileManager::kExeAdjacent,"hist.txt");
      std::ofstream out(path.c_str());
      _history->save(out);
   }

   // return to normalcy
   _pen.str() << pen::fgcol(pen::kDefault) << pen::bgcol(pen::kDefault);// << pen::showCursor();
}

void intCommand::setupDefaults(resolve::iProgramResolver& r)
{
   r.addBuiltIn("q");
   r.adoptFromCmdExe("dir");
   r.adoptFromCmdExe("tasklist");
   r.adoptFromCmdExe("tree");

   tcat::typePtr<ledit::iCmdHelp> pHlp;

   pHlp->addStartsWithHelp("cmd",      "[/CK] <expr>");
   pHlp->addExactHelp(     "cmd /",    "<C termintate> <K remain>");

   pHlp->addStartsWithHelp("dir",      "[/AS] <path>");
   pHlp->addExactHelp(     "dir /",    "<A:-HS attribute> <S recurse>");

   pHlp->addStartsWithHelp("find",     "<path> -iname <expr>");

   pHlp->addStartsWithHelp("grep",     "[-rniI] <expr> <path>");
   pHlp->addExactHelp(     "grep -",   "<r recursive> <n line numbers> <i case insens> <I skip binaries>");

   pHlp->addStartsWithHelp("ls",       "[-laRX] <path>");
   pHlp->addExactHelp(     "ls -",     "<l list fmt> <a all files> <R recursive> <X sort by ext>");

   pHlp->addStartsWithHelp("tasklist", "[/FI \"imagename eq <expr>\"]");

   pHlp->addStartsWithHelp("tree",     "<path> [/F]");
}

} // anonymous namespace
