#include "animation.h"

#include <utility>

namespace Hypercube {

Animation::Animation(int type, int left_frames) : type_(type), total_frames_(left_frames), left_frames_(left_frames), tag_(0) {}

int Animation::type() const { return type_; }

int Animation::total_frames() const { return total_frames_; }

int Animation::left_frames() const { return left_frames_; }

void Animation::Update() {
    if (left_frames_ > 0) left_frames_--;
}

int Animation::tag() const { return tag_; }

void Animation::set_tag(int tag) { tag_ = tag; }

int Animation::tag_id() const { return tag_id_; }

void Animation::set_tag_id(int tag_id) { tag_id_ = tag_id; }

}  // namespace Hypercube
