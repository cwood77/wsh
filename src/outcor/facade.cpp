#include "../cmn/autoPtr.hpp"
#include "../cmn/win32.hpp"
#include "../exec/api.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <memory>

namespace outcor {

class pipeThread : public cmn::iThread {
public:
   pipeThread(exec::iOutPipe& p, iOutCorrelator& o, bool isOut)
   : m_pipe(p), m_out(o), m_isOut(isOut) {}

   virtual void run()
   {
      m_pipe.processLoop([&](auto& s)
      {
         cmn::autoReleasePtr<outcor::iSink> pSink(&m_out.lock(m_isOut));
         pSink->write(0,s);
      });
   }

private:
   exec::iOutPipe& m_pipe;
   iOutCorrelator& m_out;
   const bool m_isOut;
};

class subprocessFacadeImpl {
public:
   subprocessFacadeImpl(iOutCorrelator& o)
   : m_outTh(*m_pStdOut,o,true)
   , m_errTh(*m_pStdErr,o,false)
   , m_outTc(m_outTh)
   , m_errTc(m_errTh)
   {}

   void beginExecute(const std::string& command)
   {
      m_outTc.start();
      m_errTc.start();
      tcat::typePtr<exec::iProcessRunner> pProc;
      pProc->execute(NULL,command.c_str(),&*m_pStdOut,&*m_pStdErr);
   }

   void join()
   {
      m_outTc.join();
      m_errTc.join();
   }

private:
   tcat::typePtr<exec::iOutPipe> m_pStdOut;
   tcat::typePtr<exec::iOutPipe> m_pStdErr;

   pipeThread m_outTh;
   pipeThread m_errTh;

   cmn::threadController m_outTc;
   cmn::threadController m_errTc;
};

class subprocessFacade : public iSubprocessFacade {
public:
   virtual void beginExecute(iOutCorrelator& o, const std::string& command)
   {
      m_pImpl.reset(new subprocessFacadeImpl(o));
      m_pImpl->beginExecute(command);
   }

   virtual void join()
   {
      m_pImpl->join();
   }

private:
   std::unique_ptr<subprocessFacadeImpl> m_pImpl;
};

tcatExposeTypeAs(subprocessFacade,iSubprocessFacade);

} // namespace outcor
