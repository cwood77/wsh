#ifndef ___cui_api___
#define ___cui_api___

#include <cstdlib>
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

} // namespace cui

#endif // ___cui_api___
