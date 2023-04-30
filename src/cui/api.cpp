#define WIN32_LEAN_AND_MEAN
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <conio.h>
#include <windows.h>

namespace cui { 

class userInput : public iUserInput {
public:
   virtual void flush() { while(::kbhit()) { ::getch(); } }
   virtual char getKey() { return ::getch(); }
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

} // namespace cui

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
