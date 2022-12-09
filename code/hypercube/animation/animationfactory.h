#ifndef HYPERCUBE_ANIMATIONFACTORY_H
#define HYPERCUBE_ANIMATIONFACTORY_H

#include "animation.h"
#include "animationfall.h"
#include "animationremove.h"
#include "animationswap.h"
#include "animationwait.h"

namespace Hypercube {

class AnimationFactory {
   public:
    static AnimationFactory& GetInstance();

    Animation* GetAnimation(int type, int frames = -1, int id1 = -1, int id2 = -1);

   public:
    static constexpr int kAnimationEmpty = 0;
    static constexpr int kAnimationFall = 1;    // left_frames
    static constexpr int kAnimationSwap = 2;    // left_frames, id1, id2
    static constexpr int kAnimationRemove = 3;  // left_frames, id
    static constexpr int kAnimationWait = 4;    // left_frames

   private:
    AnimationFactory();
};

}  // namespace Hypercube

#endif  // HYPERCUBE_ANIMATIONFACTORY_H
