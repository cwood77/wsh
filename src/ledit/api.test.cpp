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
#include <set>
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

   virtual bool hasShift() { return shifts.find(i-1)!=shifts.end(); }
   virtual bool hasControl() { return false; }

   std::vector<char> keys;
   size_t i;
   std::set<size_t> shifts;
};

void testLineEditor(std::function<void(void)>f )
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
   tcat::typePtr<ledit::iCmdLineHistory> _history;
   cmn::autoService<ledit::iCmdLineHistory> _historySvc(*svcMan,*_history);

   f();
}

testDefineTest(ledit_basicHandler_lettersBackspaceEnter)
{
   testLineEditor([&]()
   {
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

      tcat::typePtr<cmn::serviceManager> svcMan;
      cmn::autoService<cui::iUserInput> _inSvc(*svcMan,in);

      tcat::typePtr<ledit::iCmdLineEditor> ledit;
      auto ans = ledit->run();
      //std::cout << "ACTUAL<" << ans << ">" << std::endl;;
      a.assertTrue(ans.resolvedCommand == "hello world");
   });
}

void testTabCompletion(test::iAsserter& a, size_t nTabs, size_t nShiftTabs, const std::string& expected)
{
   testLineEditor([&]()
   {
      fakeKeys in;
      in.keys.push_back('b');
      in.keys.push_back('e');
      in.keys.push_back('f');
      in.keys.push_back('o');
      in.keys.push_back('r');
      in.keys.push_back('e');
      in.keys.push_back(' ');
      in.keys.push_back('s');
      in.keys.push_back('r');
      in.keys.push_back('c');
      in.keys.push_back('\\');
      in.keys.push_back(' ');
      in.keys.push_back('a');
      in.keys.push_back('f');
      in.keys.push_back('t');
      in.keys.push_back('e');
      in.keys.push_back('r');

      for(size_t i=0;i<8;i++) // land on 'c'
      {
         in.keys.push_back(-32);
         in.keys.push_back(75); // left arrow
      }

      for(size_t i=0;i<nTabs;i++)
         in.keys.push_back(9); // tab

      for(size_t i=0;i<nShiftTabs;i++)
      {
         in.shifts.insert(in.keys.size());
         in.keys.push_back(9); // tab
      }

      in.keys.push_back(13); // enter

      tcat::typePtr<cmn::serviceManager> svcMan;
      cmn::autoService<cui::iUserInput> _inSvc(*svcMan,in);

      tcat::typePtr<ledit::iCmdLineEditor> ledit;
      auto ans = ledit->run();
      //std::cout << "ACTUAL<" << ans.userText << ">" << std::endl;
      a.assertTrue(ans.resolvedCommand == expected);
   });
}

testDefineTest(ledit_fileHandler_1Tab)
{
   testTabCompletion(a,/*nTabs*/1,/*nShiftTabs*/0,"before src\\childTest after");
}

testDefineTest(ledit_fileHandler_3Tab)
{
   testTabCompletion(a,/*nTabs*/3,/*nShiftTabs*/0,"before src\\console after");
}

testDefineTest(ledit_fileHandler_1rTab)
{
   testTabCompletion(a,/*nTabs*/3,/*nShiftTabs*/1,"before src\\cmn after");
}

testDefineTest(ledit_fileHandler_infTab)
{
   testTabCompletion(a,/*nTabs*/100,/*nShiftTabs*/0,"before src\\wsh after");
}

testDefineTest(ledit_fileHandler_infTab1rTab)
{
   testTabCompletion(a,/*nTabs*/100,/*nShiftTabs*/1,"before src\\test after");
}

} // anonymous namespace

#endif // cdwTest

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
