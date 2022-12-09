#include "animationremove.h"

namespace Hypercube {

AnimationRemove::AnimationRemove(int type, int left_frames, int id) : Animation(type, left_frames), id_(id) {}

int AnimationRemove::id() const { return id_; }

}  // namespace Hypercube
