#define WIN32_LEAN_AND_MEAN
#include "../cmn/win32.hpp"
#include "windows.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// --siblings X
// --hang
// --set Y
// --recurse Z
//
// example:
// --recurse 3 --siblings 2 --set mvVar

int         gRecurse  = 0;
int         gSiblings = 0;
const char *gSet      = NULL;
bool        gHang     = false;
const char *gSelf     = NULL;

static void parseArgs(int argc, const char *argv[])
{
   if(argc == 1)
      throw std::runtime_error("bad usage");

   for(int i=1;i<argc;i++)
   {
      if(::strcmp(argv[i],"--recurse")==0)
         ::sscanf(argv[++i],"%d",&gRecurse);
      else if(::strcmp(argv[i],"--siblings")==0)
         ::sscanf(argv[++i],"%d",&gSiblings);
      else if(::strcmp(argv[i],"--set")==0)
         gSet = argv[++i];
      else if(::strcmp(argv[i],"--hang")==0)
         gHang = true;
      else
         throw std::runtime_error("bad usage 2");
   }

   gSelf = argv[0];
}

static void printBanner()
{
   std::cout << "childTest " << ::GetCurrentProcessId() << " {";
   std::cout << gRecurse << "," << gSiblings << "," << (gSet?gSet:"") << "," << gHang << "}";
   std::cout << std::endl;
}

static void spawn(int recurse, int siblings, const char *set, bool hang)
{
   std::stringstream stream;
   stream << gSelf << " ";

   if(recurse)
      stream << "--recurse " << recurse << " ";

   if(siblings)
      stream << "--siblings " << siblings << " ";

   if(set)
      stream << "--set " << set << " ";

   if(hang)
      stream << "--hang";

   ::system(stream.str().c_str());
}

int main(int argc, const char *argv[])
{
   try
   {
      parseArgs(argc,argv);
      printBanner();

      // handle siblings
      for(int i=0;i<gSiblings;i++)
         spawn(0,0,NULL,true);

      // handle recursion
      if(gRecurse)
         spawn(gRecurse-1,gSiblings,gSet,gHang);

      // set implies conditional
      if(gRecurse && gSet)
         gSet = NULL;

      // handle set/hang
      if(gSet)
      {
         std::cout << "setting '" << gSet << "'" << std::endl;
         cmn::osEvent(gSet).set();
      }
      if(gHang)
      {
         std::cout << "hanging process " << ::GetCurrentProcessId() << std::endl;
         while(true)
            ::Sleep(1000);
      }
      else
         std::cout << "exiting process " << ::GetCurrentProcessId() << std::endl;
   }
   catch(std::exception& x)
   {
      std::cerr << "ERROR: " << x.what() << std::endl;
      return -1;
   }
   return 0;
}
