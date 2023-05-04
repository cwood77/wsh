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
   , readyToSend(false), lastNonPromptLength(0), iProcEnd(-1), lastHelpLength(0) {}

   cui::pnt loc;
   std::string prompt;
   std::string userText;
private: // unused
   iCompletionState *pCompletionState;
public: // used
   int iCursor;
private: // unused
   int iHintStart;
   iGuessState *pGuessState;
public: // used
   std::string helpText;
   bool readyToSend;
   size_t lastNonPromptLength;
private: // unused
   int iProcEnd; // divisor between process and arguments
public: // used

   std::string resolved;
   std::string pwd;

   size_t lastHelpLength;
};

class iCmdLineEditor {
public:
   virtual ~iCmdLineEditor() {}
   virtual std::string run() = 0;
};

class extKey {
public:
   extKey() : mod(0), base(0) {}

   bool is(char c) const { return mod == 0 && base == c; }
   bool modIs(char c) const { return mod == -32 && base == c; }

   char mod;
   char base;
};

class iCmdLineKeyHandler {
public:
   virtual ~iCmdLineKeyHandler() {}
   virtual bool tryHandle(extKey c, cmdLineState& s) = 0;
   virtual bool tryHandleLast(extKey c, cmdLineState& s) = 0;
};

class iCmdHelp {
public:
   virtual ~iCmdHelp() {}

   virtual void addStartsWithHelp(const std::string& match, const std::string& help) = 0;
   virtual void addExactHelp(const std::string& match, const std::string& help) = 0;

   virtual std::string getHelp(cmdLineState& s) = 0;
};

} // namespace ledit

#endif // ___ledit_api___
