#define WIN32_LEAN_AND_MEAN
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include "pen.hpp"
#include <conio.h>
#include <windows.h>

namespace cui { 

class userInput : public iUserInput {
public:
   virtual void flush() { while(::kbhit()) { ::getch(); } }
   virtual char getKey() { return ::getch(); }
   virtual bool hasShift() { return ::GetAsyncKeyState(VK_SHIFT) & ~0x1; }
};

tcatExposeTypeAs(userInput,iUserInput);

class cannedUserInput : public iCannedUserInput {
public:
   cannedUserInput() : m_i(0), m_pNext(NULL) {}

   virtual void flush() {}

   virtual char getKey()
   {
      if(m_i < m_input.size())
      {
         ::Sleep(1*1000);
         return m_input[m_i++];
      }
      else
         return m_pNext->getKey();
   }

   virtual bool hasShift() { return m_pNext->hasShift(); }

   virtual void chain(iUserInput& inner) { m_pNext = &inner; }

   virtual void configure(const std::vector<char>& in)
   {
      m_input = in;
      m_i = 0;
   }

private:
   std::vector<char> m_input;
   size_t m_i;
   iUserInput *m_pNext;
};

tcatExposeTypeAs(cannedUserInput,iCannedUserInput);

class stylePrefs : public iStylePrefs {
public:
   virtual void set(styles s, std::ostream& o)
   {
      switch(s)
      {
         case kPrompt:
            o << pen::fgcol(pen::kYellow);
            break;
         default:
         case kNormal:
            o << pen::fgcol(pen::kDefault);
            break;
         case kHint:
            o << pen::fgcol(pen::kBlack,true);
            break;
         case kHelp:
            o << pen::fgcol(pen::kBlue,true);
            break;
         case kError:
            o << pen::fgcol(pen::kRed,true);
            break;
         case kPwd:
            o << pen::fgcol(pen::kGreen);
            break;
      }
   }
};

tcatExposeTypeAs(stylePrefs,iStylePrefs);

class styler : public iStyler {
public:
   virtual void bind(iStylePrefs& s, std::ostream& p)
   {
      m_pPrefs = &s;
      m_pStream = &p;
   }

   virtual iStyler& with(iStylePrefs::styles s, std::function<void(std::ostream&)> f)
   {
      m_pPrefs->set(s,*m_pStream);
      try
      {
         f(*m_pStream);
      }
      catch(...)
      {
         m_pPrefs->set(iStylePrefs::kNormal,*m_pStream);
         throw;
      }
      return *this;
   }

private:
   iStylePrefs *m_pPrefs;
   std::ostream *m_pStream;
};

tcatExposeTypeAs(styler,iStyler);

} // namespace cui

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
