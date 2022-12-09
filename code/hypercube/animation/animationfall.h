#ifndef HYPERCUBE_FALLANIMATION_H
#define HYPERCUBE_FALLANIMATION_H

#include "animation.h"

namespace Hypercube {

class AnimationFall : public Animation {
   public:
    AnimationFall(int type, int left_frames);
};

}  // namespace Hypercube

#endif  // HYPERCUBE_FALLANIMATION_H
