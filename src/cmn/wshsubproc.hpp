#ifndef ___cmn_wshsubproc___
#define ___cmn_wshsubproc___

#include <cstring>
#include <sstream>
#include <string>

// subprocess API
//
// There are NO built-in commands: they are all external processes.
// This compartmentalizes the code, and also allows commands to be re-used in multiple
// shells as much as possible.
// Commands need
//  (1) to know when they are running under wsh, and
//  (2) how to communicatae back to wsh for outputs, including
//      - changing the directory
//      - manipulating envvars
//      - quitting the shell
//
// (1) I may punt on, or tie as a sideffect of (2).
// (2) is done through shmem
//
// subprocess ID -> shmemblock that contains shell procId and channelId
// shell block is just a set of paths in TMP

namespace cmn {

struct wshSubprocBlock { // 64-bytes
public:
   char   masterName[56];
   size_t channel;
};

struct wshChannelBlock {
public:
   unsigned long flags;
   char          filePath[1024];
   char          cmd[1024];

   bool isClear() const
   {
      return cmd[0] == 0;
   }

   void clear()
   {
      flags = 0;
      ::memset(filePath,0,1024);
      ::memset(cmd,0,1024);
   }
};

struct wshMasterBlock {
public:
   enum { kCurrentVersion = 1 };

   char            version;
   wshChannelBlock channels[1];
};

inline std::string buildWshMasterShmemName(size_t procId)
{
   std::stringstream stream;
   stream << "cdw_wsh_master_shmem:" << procId;
   return stream.str();
}

inline std::string buildWshSubprocShmemName(size_t procId)
{
   std::stringstream stream;
   stream << "cdw_wsh_subproc_shmem:" << procId;
   return stream.str();
}

class iWshTailCommand {
public:
   virtual ~iWshTailCommand() {}
   virtual void execute(wshChannelBlock& b) = 0;
};

class wshQuitException : public std::exception {};

} // namespace cmn

#endif // ___cmn_wshsubproc___
