#include "animation.h"

#include <utility>

namespace Hypercube {

Animation::Animation(int type, int left_frames) : type_(type), total_frames_(left_frames), left_frames_(left_frames) {}

int Animation::type() const { return type_; }

int Animation::total_frames() const { return total_frames_; }

int Animation::left_frames() const { return left_frames_; }

void Animation::Update() {
    if (left_frames_ > 0) left_frames_--;
}

}  // namespace Hypercube
