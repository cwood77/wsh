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

// [--prompt--][--usertext--][--hint--]
// <help>
class cmdLineState {
public:
   cmdLineState()
   : loc(0,0), pCompletionState(NULL), iCursor(0), iHintStart(0), pGuessState(NULL)
   , readyToSend(false), lastNonPromptLength(0) {}

   cui::pnt loc;
   std::string prompt;
   std::string userText;
   iCompletionState *pCompletionState;
   int iCursor;
   int iHintStart;
   iGuessState *pGuessState;
   std::string helpText;
   bool readyToSend;
   size_t lastNonPromptLength;
};

class iCmdLineEditor {
public:
   virtual ~iCmdLineEditor() {}
   virtual std::string run() = 0;
};

class iCmdLineKeyHandler {
public:
   virtual ~iCmdLineKeyHandler() {}
   virtual bool tryHandle(char c, cmdLineState& s) = 0;
   virtual bool tryHandleLast(char c, cmdLineState& s) = 0;
};

} // namespace ledit

#endif // ___ledit_api___
