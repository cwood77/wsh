#define WIN32_LEAN_AND_MEAN
#include "../cmn/win32.hpp"
#include "../cmn/wshsubproc.hpp"
#include "tailCmd.hpp"
#include <iostream>
#include <stdexcept>
#include <windows.h>

int main(int argc, const char *argv[])
{
   try
   {
      cmn::shmem<cmn::wshSubprocBlock> pShmem(
         cmn::buildWshSubprocShmemName(::GetCurrentProcessId()));
      if(!pShmem.existed())
         throw std::runtime_error("don't appear to be running under wsh?");

      cmn::shmem<cmn::wshMasterBlock> pMaster(pShmem->masterName);
      if(!pShmem.existed())
         throw std::runtime_error("can't open comms channel with wsh");

      auto& channel = pMaster->channels[pShmem->channel];
      ::strcpy(channel.cmd,typeid(q::qTailCommand).name());
   }
   catch(std::exception& x)
   {
      std::cerr << "ERROR: " << x.what() << std::endl;
      return -1;
   }
   return 0;
}
