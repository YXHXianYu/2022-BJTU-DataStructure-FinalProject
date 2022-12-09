#ifndef HYPERCUBE_WAITANIMATION_H
#define HYPERCUBE_WAITANIMATION_H

#include "animation.h"

namespace Hypercube {

class AnimationWait : public Animation {
   public:
    AnimationWait(int type, int left_frames);
};

}  // namespace Hypercube

#endif  // HYPERCUBE_WAITANIMATION_H
