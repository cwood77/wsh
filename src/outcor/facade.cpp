#include "../cmn/autoPtr.hpp"
#include "../cmn/service.hpp"
#include "../cmn/win32.hpp"
#include "../cmn/wshsubproc.hpp"
#include "../cui/api.hpp"
#include "../exec/api.hpp"
#include "../exec/cancel.hpp"
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
   , m_masterShmem(m_pSvcMan->demand<cmn::wshMasterBlock>())
   , m_cancel(m_pSvcMan->demand<cancel::iKeyMonitor>())
   {}

   void beginExecute(const std::string& command)
   {
      m_cancelMon.reset(new cancel::autoInstallMonitor(m_cancel));
      m_outTc.start();
      m_errTc.start();
      tcat::typePtr<exec::iProcessRunner> pProc;
      pProc->execute(&*m_pJob,command.c_str(),&*m_pStdOut,&*m_pStdErr,[&](DWORD procId)
      {
         // assign a channel for this process in case it wants to use shmem
         size_t myIdx = 0;
         m_masterShmem.channels[myIdx].clear();
         m_pShmem.reset(new cmn::shmem<cmn::wshSubprocBlock>(
            cmn::buildWshSubprocShmemName(procId)));
         ::strcpy(
            (*m_pShmem)->masterName,
            cmn::buildWshMasterShmemName(::GetCurrentProcessId()).c_str());
         (*m_pShmem)->channel = myIdx;
      });
   }

   void join()
   {
      // handle cancel
      bool wasCancelled = false;
      m_cancel.waitForCancelUntil(m_outTc.getHandle(),[&]()
      {
         auto& styler = m_pSvcMan->demand<cui::iStyler>();
         styler.error([](auto& o){ o << std::endl << "user Ctrl-C aborting process" << std::endl; });
         m_pJob->terminate();
         wasCancelled = true;
      });
      m_cancelMon.reset(NULL);
      m_outTc.join();
      m_errTc.join();

      if(wasCancelled)
      {
         auto& styler = m_pSvcMan->demand<cui::iStyler>();
         styler.error([](auto& o){ o << std::endl << "killed" << std::endl; });
         return;
      }

      // examine shmem for tail processing
      size_t myIdx = (*m_pShmem)->channel;
      auto& channel = m_masterShmem.channels[myIdx];
      if(!channel.isClear())
      {
         tcat::typePtr<cmn::iWshTailCommand> pCmd(channel.cmd);
         pCmd->execute(channel);
      }
   }

private:
   tcat::typePtr<cmn::serviceManager> m_pSvcMan;
   tcat::typePtr<exec::iOutPipe> m_pStdOut;
   tcat::typePtr<exec::iOutPipe> m_pStdErr;
   tcat::typePtr<exec::iJob> m_pJob;

   pipeThread m_outTh;
   pipeThread m_errTh;

   cmn::threadController m_outTc;
   cmn::threadController m_errTc;

   cmn::wshMasterBlock& m_masterShmem;
   std::unique_ptr<cmn::shmem<cmn::wshSubprocBlock> > m_pShmem;

   cancel::iKeyMonitor& m_cancel;
   std::unique_ptr<cancel::autoInstallMonitor> m_cancelMon;
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
