#ifndef ___ledit_api___
#define ___ledit_api___

#include "../cui/api.hpp"

namespace ledit {

class iCompletionState {
public:
   virtual ~iCompletionState() {}
   virtual void release() = 0;

   virtual void next() = 0;
   virtual void prev() = 0;
   virtual std::string getSuffix() = 0;
};

class iGuessState {
public:
   virtual ~iGuessState() {}
   virtual void release() = 0;

   virtual std::string advanceWord() = 0;
   virtual std::string advanceAll() = 0;
};

class cmdLineState {
public:
   cmdLineState()
   : loc(0,0), pCompletionState(NULL), iCursor(0), iHintStart(0), pGuessState(NULL)
   , readyToSend(false) {}

   cui::pnt loc;
   std::string prompt;
   std::string userText;
   iCompletionState *pCompletionState;
   int iCursor;
   int iHintStart;
   iGuessState *pGuessState;
   std::string helpText;
   bool readyToSend;
};

class iCmdLineEditor {
public:
   virtual ~iCmdLineEditor() {}
   virtual void run() = 0;
};

class iCmdLineKeyHandler {
public:
   virtual bool tryHandle(char c, cmdLineState& s) = 0;
};

} // namespace ledit

#endif // ___ledit_api___
