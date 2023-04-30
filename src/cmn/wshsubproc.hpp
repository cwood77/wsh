#ifndef ___cmn_wshsubproc___
#define ___cmn_wshsubproc___

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
   char masterName[56];
   size_t channel;
};

struct wshChannelBlock {
public:
   unsigned long flags;
   char filePath[1024];
};

struct wshMasterBlock {
public:
   char version;
   wshChannelBlock[1];
};

} // namespace cmn

#endif // ___cmn_wshsubproc___
