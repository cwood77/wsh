#ifndef ___cui_api___
#define ___cui_api___

#include <cstdlib>
#include <functional>
#include <ostream>
#include <vector>

namespace cui { 

// --------------- simple atomics

class pnt {
public:
   pnt(size_t x, size_t y) : x(x), y(y) {}

   size_t x;
   size_t y;
};

class iUserInput {
public:
   virtual ~iUserInput() {}
   virtual void flush() = 0;
   virtual char getKey() = 0;
};

class iCannedUserInput : public iUserInput {
public:
   virtual void chain(iUserInput& inner) = 0;
   virtual void configure(const std::vector<char>& in) = 0;
};

// --------------- prompt

class iStylePrefs {
public:
   enum styles {
      kPrompt,
      kNormal,
      kHint,
      kHelp,
      kError,
      kPwd
   };

   virtual ~iStylePrefs() {}
   virtual void set(styles s, std::ostream& o) = 0;
};

class iStyler {
public:
   virtual ~iStyler() {}

   virtual void bind(iStylePrefs& s, std::ostream& p) = 0;
   virtual iStyler& with(iStylePrefs::styles s, std::function<void(std::ostream&)> f) = 0;

   iStyler& prompt(std::function<void(std::ostream&)> f)
   { return with(iStylePrefs::kPrompt,f); }
   iStyler& normal(std::function<void(std::ostream&)> f)
   { return with(iStylePrefs::kNormal,f); }
   iStyler& hint(std::function<void(std::ostream&)> f)
   { return with(iStylePrefs::kHint,f); }
   iStyler& help(std::function<void(std::ostream&)> f)
   { return with(iStylePrefs::kHelp,f); }
   iStyler& error(std::function<void(std::ostream&)> f)
   { return with(iStylePrefs::kError,f); }
   iStyler& pwd(std::function<void(std::ostream&)> f)
   { return with(iStylePrefs::kPwd,f); }
};

// --------------- handling

} // namespace cui

#endif // ___cui_api___
