#include "animationfactory.h"

#include <cassert>

namespace Hypercube {

AnimationFactory& AnimationFactory::GetInstance() {
    static AnimationFactory instance;
    return instance;
}

Animation* AnimationFactory::GetAnimation(int type, int frames, int id1, int id2) {
    Animation* animation = nullptr;
    switch (type) {
        case kAnimationEmpty: {
            animation = new Animation(type, 0);
            break;
        }
        case kAnimationFall: {
            animation = new AnimationFall(type, frames);
            break;
        }
        case kAnimationSwap: {
            if (frames == -1 || id1 == -1 || id2 == -1) assert(false);
            animation = new AnimationSwap(type, frames, id1, id2);
            break;
        }
        case kAnimationRemove: {
            if (frames == -1 || id1 == -1) assert(false);
            animation = new AnimationRemove(type, frames, id1);
            break;
        }
        case kAnimationWait: {
            if (frames == -1) assert(false);
            animation = new AnimationWait(type, frames);
            break;
        }
    }
    return animation;
}

AnimationFactory::AnimationFactory() {}

}  // namespace Hypercube
