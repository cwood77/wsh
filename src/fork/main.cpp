#include "../cmn/win32.hpp"
#include "../exec/api.hpp"
#include "../tcatlib/api.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, const char *argv[])
{
   tcat::typePtr<exec::iProcessRunner> pExec;
   try
   {
      if(argc != 2)
         throw std::runtime_error("usage: fork <command>");

      cmn::autoHandle h;
      h.h = pExec->execute(NULL,argv[1],NULL,NULL,[](auto pid){});
   }
   catch(std::exception& x)
   {
      std::cerr << "ERROR: " << x.what() << std::endl;
      return -1;
   }
   return 0;
}
