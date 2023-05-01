#ifndef ___exec_cancel___
#define ___exec_cancel___

#define WIN32_LEAN_AND_MEAN
#include "../cmn/service.hpp"
#include "windows.h"

namespace cancel {

class iKeyMonitor {
public:
   virtual ~iKeyMonitor() {}

   virtual void install(bool add) = 0;
   virtual void recordThreadIo(HANDLE h, bool add) = 0;

   virtual bool wasAborted() const = 0;
};

class autoInstallKeyMonitor {
public:
   explicit autoInstallKeyMonitor(iKeyMonitor& k) : m_k(k) { m_k.install(true); }
   ~autoInstallKeyMonitor() { m_k.install(false); }

private:
   iKeyMonitor& m_k;
};

class autoInstallSyncIo {
public:
   autoInstallSyncIo(iKeyMonitor& k, HANDLE h) : m_k(k), m_h(h)
   { m_k.recordThreadIo(m_h,true); }
   ~autoInstallSyncIo() { m_k.recordThreadIo(m_h,false); }

private:
   iKeyMonitor& m_k;
   HANDLE m_h;
};

} // namespace cancel

#endif // ___exec_cancel___
