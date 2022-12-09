#ifndef HYPERCUBE_REMOVEANIMATION_H
#define HYPERCUBE_REMOVEANIMATION_H

#include "animation.h"

namespace Hypercube {

class AnimationRemove : public Animation {
   public:
    AnimationRemove(int type, int left_frames, int id);

    int id() const;

   private:
    int id_;
};

}  // namespace Hypercube

#endif  // HYPERCUBE_REMOVEANIMATION_H
