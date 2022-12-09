#include "animationswap.h"

namespace Hypercube {

AnimationSwap::AnimationSwap(int type, int left_frames, int id1, int id2) : Animation(type, left_frames), id1_(id1), id2_(id2) {}

int AnimationSwap::getId1() const { return id1_; }

int AnimationSwap::getId2() const { return id2_; }

}  // namespace Hypercube
