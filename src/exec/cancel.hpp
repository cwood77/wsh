#ifndef ___exec_cancel___
#define ___exec_cancel___

#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <functional>

namespace cancel {

class iKeyMonitor {
public:
   virtual ~iKeyMonitor() {}

   virtual void install(bool add) = 0;

   virtual void waitForCancelUntil(HANDLE h, std::function<void(void)> f) = 0;
   virtual void clearAborted() = 0;
};

class autoInstallMonitor {
public:
   explicit autoInstallMonitor(iKeyMonitor& k) : m_k(k)
   { m_k.clearAborted(); m_k.install(true); }
   ~autoInstallMonitor() { m_k.install(false); }

private:
   iKeyMonitor& m_k;
};

} // namespace cancel

#endif // ___exec_cancel___
