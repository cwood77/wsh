#ifndef ___q_tailCmd___
#define ___q_tailCmd___

#include "../cmn/wshsubproc.hpp"

namespace q {

class qTailCommand : public cmn::iWshTailCommand {
public:
   virtual void execute(cmn::wshChannelBlock&)
   {
      throw cmn::wshQuitException();
   }
};

} // namespace q

#endif // ___q_tailCmd___
