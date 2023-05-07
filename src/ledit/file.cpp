#define WIN32_LEAN_AND_MEAN
#include "../cmn/win32.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <windows.h>

namespace ledit {
namespace {

class stateEntry {
public:
   stateEntry() : idx(0) {}

   std::string origWord;

   std::string before;
   std::string word;
   std::string after;
   size_t idx;
};

class fileCompletionState : public iFileCompletionState {
public:
   std::map<std::string,stateEntry> entries;
};

} // anonymous namespace

class fileCmdLineKeyHandler : public iCmdLineKeyHandler {
public:
   virtual bool tryHandle(extKey c, cmdLineState& s)
   {
      bool doSearch = false;
      bool up = true;

      if(c.is(9)) // tab
      {
         doSearch = true;
         up = true;
      }
      else if(c.isShift(9)) // tab
      {
         doSearch = true;
         up = false;
      }

      if(doSearch)
      {
         if(!s.pFileCompletionState)
            s.pFileCompletionState = new fileCompletionState();
         auto& map = dynamic_cast<fileCompletionState&>(*s.pFileCompletionState).entries;

         auto it = map.find(s.userText);
         if(it == map.end())
         {
            stateEntry se;
            findWord(s,se.before,se.origWord,se.after);
            se.word = se.origWord;
            bool hit = tryFindPath(se.word,se.idx);
            if(hit)
            {
               //adjustIndex(se.idx,up);
               s.userText = se.before + se.word + se.after;
               s.iCursor = se.before.length() + se.word.length();
               map[s.userText] = se;
            }
         }
         else
         {
            stateEntry& se = it->second;
            se.word = se.origWord;
            adjustIndex(se.idx,up);
            bool hit = tryFindPath(se.word,se.idx);
            if(hit)
            {
               s.userText = se.before + se.word + se.after;
               s.iCursor = se.before.length() + se.word.length();
               map[s.userText] = se;
            }
         }

         return true;
      }

      return false;
   }

   virtual bool tryHandleLast(extKey c, cmdLineState& s) { return false; }

private:
   void findWord(cmdLineState& s,
      std::string& before, std::string& word, std::string& after)
   {
      {
         const char *pThumb = s.userText.c_str() + s.iCursor;
         for(;pThumb!=s.userText.c_str()&&*pThumb!=' ';--pThumb);
         before = std::string(s.userText.c_str(),pThumb-s.userText.c_str());
         if(before.length())
            before+= " ";
      }

      {
         const char *pThumb = s.userText.c_str() + s.iCursor;
         for(;*pThumb!=0&&*pThumb!=' ';++pThumb);
         after = pThumb;
      }

      word = std::string(
         s.userText.c_str()+before.length(),
         s.userText.length()-before.length()-after.length());
   }

   bool tryFindPath(std::string& word, size_t& idx)
   {
      WIN32_FIND_DATA fData;
      cmn::autoFindHandle hFind;
      hFind.h = ::FindFirstFileA((word + "*").c_str(),&fData);
      if(hFind.h == INVALID_HANDLE_VALUE)
         return false;

      size_t i=0;
      std::string lastAnswer;
      do
      {
         if(std::string(".") == fData.cFileName)
            continue;
         if(std::string("..") == fData.cFileName)
            continue;

         if(i == idx)
         {
            rebuildWord(word,fData.cFileName);
            return true;
         }
         else
            lastAnswer = fData.cFileName;

         i++;

      } while(::FindNextFileA(hFind.h,&fData));

      if(!lastAnswer.empty())
      {
         rebuildWord(word,fData.cFileName);
         idx = (i-1);
         return true;
      }
      else
         return false;
   }

   void rebuildWord(std::string& word, const std::string& fileName)
   {
      auto *pThumb = ::strrchr(word.c_str(),'\\');
      if(pThumb)
      {
         std::string before(word.c_str(),pThumb-word.c_str()+1);
         before += fileName;
         word = before;
      }
      else
         word = fileName;
   }

   void adjustIndex(size_t& idx, bool up)
   {
      if(up)
         idx++;
      else if(idx)
         idx--;
   }
};

tcatExposeTypeAs(fileCmdLineKeyHandler,iCmdLineKeyHandler);

} // namespace ledit
