#ifndef ___outcor_api___
#define ___outcor_api___

#include "../ledit/api.hpp"
#include <string>

namespace outcor {

class iSink {
public:
   virtual ~iSink() {}
   virtual void release() = 0;
   virtual void write(time_t ts, const std::string& s) = 0;
};

class iOutCorrelator {
public:
   virtual ~iOutCorrelator() {}
   virtual iSink& lock(bool isOut) = 0;
};

class iSubprocessFacade {
public:
   virtual ~iSubprocessFacade() {}
   virtual void beginExecute(iOutCorrelator& o, const ledit::cmdLineResult& command) = 0;
   virtual bool join() = 0;
};

class iOutputColorer {
public:
   virtual ~iOutputColorer() {}
   virtual std::string color(const std::string& blob) = 0;
};

class iOutputColorerFactory {
public:
   virtual ~iOutputColorerFactory() {}
   virtual iOutputColorer& create(const std::string& resolvedPath, bool isOut) = 0;
};

} // namespace outcor

#endif // ___outcor_api___
