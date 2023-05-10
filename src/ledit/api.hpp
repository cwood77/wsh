#ifndef ___ledit_api___
#define ___ledit_api___

#include "../cui/api.hpp"

namespace ledit {

class iFileCompletionState {
public:
   virtual ~iFileCompletionState() {}
};

class cmdLineState {
public:
   cmdLineState()
   : loc(0,0), iCursor(0), lastNonPromptLength(0), histIdx(0)
   , pFileCompletionState(NULL), readyToSend(false) {}

   ~cmdLineState() { delete pFileCompletionState; }

   cui::pnt loc;
   std::string pwd;
   std::string prompt;
   std::string userText;
   int iCursor;
   size_t lastNonPromptLength;
   size_t histIdx;

   iFileCompletionState *pFileCompletionState;

   std::string helpText;

   bool readyToSend;
   std::string resolved;
};

class iCmdLineHistory {
public:
   virtual ~iCmdLineHistory() {}

   virtual void add(const std::string& line) = 0;
   virtual std::string get(size_t& idx, bool up) = 0;
   virtual std::string matchBest(const std::string& start) = 0;

   virtual void load(std::istream& s) = 0;
   virtual void save(std::ostream& s) = 0;
};

class cmdLineResult {
public:
   cmdLineResult() {}

   cmdLineResult(const std::string& ut, const std::string& resolved)
   : userText(ut), resolvedCommand(resolved) {}

   std::string userText;
   std::string resolvedCommand;
};

class iCmdLineResultSplitter {
public:
   virtual ~iCmdLineResultSplitter() {}
   virtual std::vector<cmdLineResult> split(cmdLineResult& r) = 0;
};

class iCmdLineEditor {
public:
   virtual ~iCmdLineEditor() {}
   virtual cmdLineResult run() = 0;
};

class extKey {
public:
   extKey() : mod(0), base(0) {}

   bool is(char c) const { return mod == 0 && base == c; }
   bool modIs(char c) const { return mod == -32 && base == c; }
   bool isShift(char c) const { return mod == 1 && base == c; }

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
