#ifndef HYPERCUBE_SWAPANIMATION_H
#define HYPERCUBE_SWAPANIMATION_H

#include "animation.h"

namespace Hypercube {

class AnimationSwap : public Animation {
   public:
    AnimationSwap(int type, int left_frames, int id1, int id2);

    int getId1() const;

    int getId2() const;

   private:
    int id1_;
    int id2_;
};

}  // namespace Hypercube

#endif  // HYPERCUBE_SWAPANIMATION_H
