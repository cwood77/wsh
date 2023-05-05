#define WIN32_LEAN_AND_MEAN
#include "../tcatlib/api.hpp"
#include <windows.h>

#ifdef cdwTest
#include "../cmn/service.hpp"
#include "../cui/api.hpp"
#include "../cui/pen.hpp"
#include "../resolve/api.hpp"
#include "../test/api.hpp"
#include "api.hpp"
#include <sstream>

namespace {

class fakeKeys : public cui::iUserInput {
public:
   fakeKeys() : i(0) {}

   virtual void flush() {}

   virtual char getKey()
   {
      if(i == keys.size())
         throw std::runtime_error("ise");
      return keys[i++];
   }

   std::vector<char> keys;
   size_t i;
};

testDefineTest(ledit_basicHandler_lettersBackspaceEnter)
{
   std::stringstream nowhere;

   tcat::typePtr<cmn::serviceManager> svcMan;
   pen::object _pen(nowhere);
   cmn::autoService<pen::object> _penSvc(*svcMan,_pen);
   tcat::typePtr<cui::iStylePrefs> _stylePrefs;
   tcat::typePtr<cui::iStyler> _styler;
   _styler->bind(*_stylePrefs,_pen.str());
   cmn::autoService<cui::iStyler> _stylerSvc(*svcMan,*_styler);
   tcat::typePtr<resolve::iProgramResolver> _res;
   cmn::autoService<resolve::iProgramResolver> _resSvc(*svcMan,*_res);

   fakeKeys in;
   in.keys.push_back('c');
   in.keys.push_back('r');
   in.keys.push_back('a');
   in.keys.push_back('p');
   in.keys.push_back(27); // clear input

   in.keys.push_back('e');
   in.keys.push_back('l');
   in.keys.push_back('o');
   in.keys.push_back(-32);
   in.keys.push_back(75); // left arrow
   in.keys.push_back('l');
   in.keys.push_back(-32);
   in.keys.push_back(71); // home
   in.keys.push_back('h');

   in.keys.push_back(-32);
   in.keys.push_back(79); // end
   in.keys.push_back(' ');
   in.keys.push_back('w');
   in.keys.push_back('o');
   in.keys.push_back('r');
   in.keys.push_back('k');
   in.keys.push_back('d');
   in.keys.push_back(-32);
   in.keys.push_back(75); // left arrow
   in.keys.push_back(8); // backspace
   in.keys.push_back('l');

   in.keys.push_back(13); // enter
   cmn::autoService<cui::iUserInput> _inSvc(*svcMan,in);

   tcat::typePtr<ledit::iCmdLineEditor> ledit;
   auto ans = ledit->run();
   //std::cout << "ACTUAL<" << ans << ">" << std::endl;;
   a.assertTrue(ans.resolvedCommand == "hello world");
}

} // anonymous namespace

#endif // cdwTest

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
