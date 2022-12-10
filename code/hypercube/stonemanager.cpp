#include "stonemanager.h"

#include <cassert>
#include <iostream>
#include <random>

#include "DEBUG.h"

namespace Hypercube {

StoneManager::StoneManager(QOpenGLFunctions_4_5_Core *func) : have_initialized_(false), gem_model_manager_(func) {}

StoneManager::~StoneManager() {}

int StoneManager::Init(int nx, int ny) {
    if (have_initialized_) return kFailureHaveInitialized;

    if (DEBUG) std::cerr << "Hypercube::StoneManager::Init - 0" << std::endl;

    nx_ = nx;
    ny_ = ny;

    if (DEBUG) std::cerr << "Hypercube::StoneManager::Init - 1" << std::endl;

    stones_.clear();

    if (DEBUG) std::cerr << "Hypercube::StoneManager::Init - 2" << std::endl;

    while (!animation_queue_.empty()) animation_queue_.pop();
    is_falling = false;

    while (!pausing_queue_.empty()) pausing_queue_.pop();
    is_pausing = false;

    if (DEBUG)
        std::cerr << "Hypercube::StoneManager::Init - 3 END: "
                  << "(" << nx_ << ", " << ny_ << ")" << std::endl;
    have_initialized_ = true;
    return kSuccess;
}

void StoneManager::Start() {
    // timer_->start(10);
}

int StoneManager::Generate(int id, int x, int y, int type, int fallen_pixel) {
    if (!have_initialized_) return kFailureHaveNotInitialized;

    if (stones_.count(id)) return kFailureIDHaveBeenUsed;

    int coordinate_x = PositionToCoordinateX(x);
    int coordinate_y = PositionToCoordinateY(y);
    int coordinate_start_y = (fallen_pixel != -1 ? coordinate_y + fallen_pixel : coordinate_y + 400);

    stones_[id] = Stone(coordinate_x, coordinate_start_y, 0, 0, type);
    stones_[id].set_rotating_speed(Stone::kRotatingSpeed);

    Animation *animation = AnimationFactory::GetInstance().GetAnimation(AnimationFactory::kAnimationFall);
    animation->set_tag(coordinate_y);
    animation->set_tag_id(id);
    animation_queue_.push(animation);

    return kSuccess;
}

int StoneManager::Remove(int id, bool playAnimation) {
    if (!have_initialized_) return kFailureHaveNotInitialized;

    if (stones_.count(id) == 0) return kFailureIDNotFound;

    animation_queue_.push(AnimationFactory::GetInstance().GetAnimation(AnimationFactory::kAnimationRemove, 0, id));
    animation_queue_.back()->set_tag(playAnimation);

    return kSuccess;
}

int StoneManager::SetRotate(int id, int rotateMode) {
    if (!have_initialized_) return kFailureHaveNotInitialized;

    if (stones_.count(id) == 0) return kFailureIDNotFound;

    switch (rotateMode) {
        case kStatic: {
            stones_[id].set_rotating_speed(Stone::kRotatingStatic);
            break;
        }
        case kRotate: {
            stones_[id].set_rotating_speed(Stone::kRotatingSpeed);
            break;
        }
        case kRotateFast: {
            stones_[id].set_rotating_speed(Stone::kRotatingSpeedFast);
            break;
        }
        case kRotateInverse: {
            stones_[id].set_rotating_speed(-1.f * Stone::kRotatingSpeed);
            break;
        }
        case kRotateFastInverse: {
            stones_[id].set_rotating_speed(-1.f * Stone::kRotatingSpeedFast);
            break;
        }
        default: {
            return kFailureArgumentError;
        }
    }

    return kSuccess;
}

int StoneManager::FallTo(int id, int tar_y) {
    if (!have_initialized_) return kFailureHaveNotInitialized;

    if (stones_.count(id) == 0) return kFailureIDNotFound;

    // stones_[id].set_falling(Stone::kFallingSpeed, PositionToCoordinateY(tar_y));
    Animation *animation = AnimationFactory::GetInstance().GetAnimation(AnimationFactory::kAnimationFall);
    animation->set_tag(PositionToCoordinateY(tar_y));
    animation->set_tag_id(id);
    animation_queue_.push(animation);

    return kSuccess;
}

int StoneManager::SwapStone(int id1, int id2) {
    if (!have_initialized_) return kFailureHaveNotInitialized;

    if (stones_.count(id1) == 0) return kFailureIDNotFound;
    if (stones_.count(id2) == 0) return kFailureIDNotFound;

    animation_queue_.push(AnimationFactory::GetInstance().GetAnimation(AnimationFactory::kAnimationSwap, 0, id1, id2));

    return kSuccess;
}

bool StoneManager::isPlayingAnimation() { return animation_queue_.size() > 0; }

int StoneManager::SetPause(bool is_pause) {
    if (is_pause == is_pausing) {
        if (is_pause == true)
            return kFailureHavePaused;
        else
            return kFailureHaveContinued;
    }

    std::vector<int> will_set_pause;
    for (auto &pr : stones_) {
        const int &id = pr.first;
        will_set_pause.push_back(id);
    }
    std::random_shuffle(will_set_pause.begin(), will_set_pause.end());
    for (auto id : will_set_pause) {
        pausing_queue_.push(std::make_pair(id, is_pause));  // 这里来限定是否为暂停
    }
    is_pausing = is_pause;
}

bool StoneManager::IsPause() const { return is_pausing; }

bool StoneManager::haveRemoveInRecentFrame() {
    if (remove_in_recent_frame_) {
        remove_in_recent_frame_ = false;
        return true;
    }
    return false;
}

bool StoneManager::haveFallInRecentFrame() {
    if (fall_in_recent_frame_) {
        fall_in_recent_frame_ = false;
        return true;
    }
    return false;
}

void StoneManager::Update() {
    if (!have_initialized_) return;

    // Rotate
    std::vector<int> is_not_active_stones_;
    for (auto &pr : stones_) {
        const int &id = pr.first;
        Stone &stone = pr.second;
        if (stone.is_active()) {
            stone.UpdateRotating();  // 旋转
            stone.UpdataRemoving();  // 删除下落（独立）
        } else {
            is_not_active_stones_.push_back(id);
        }
    }

    // 清除无用宝石
    for (auto &id : is_not_active_stones_) {
        stones_.erase(id);
    }

    while (!is_falling && !animation_queue_.empty()) {
        if (animation_queue_.front()->type() == AnimationFactory::kAnimationEmpty) {
            animation_queue_.pop();
        } else if (animation_queue_.front()->type() == AnimationFactory::kAnimationFall) {
            Animation *animation;
            while (!animation_queue_.empty() && animation_queue_.front()->type() == AnimationFactory::kAnimationFall) {
                animation = animation_queue_.front();
                animation_queue_.pop();

                int id = animation->tag_id();
                stones_[id].set_falling(Stone::kFallingSpeed, animation->tag());
            }
            is_falling = true;
            break;
        } else if (animation_queue_.front()->type() == AnimationFactory::kAnimationSwap) {
            int id1 = ((AnimationSwap *)(animation_queue_.front()))->getId1();
            int id2 = ((AnimationSwap *)(animation_queue_.front()))->getId2();

            if (animation_queue_.front()->tag() == 0) {
                stones_[id1].set_swaping(stones_[id2].x(), stones_[id2].y(), Stone::kSwapingSpeed);
                stones_[id2].set_swaping(stones_[id1].x(), stones_[id1].y(), Stone::kSwapingSpeed);
                animation_queue_.front()->set_tag(1);
            }

            if (stones_[id1].is_swaping() || stones_[id2].is_swaping()) {
                stones_[id1].UpdateSwaping();
                stones_[id2].UpdateSwaping();
            } else {
                animation_queue_.pop();
            }
            break;  // 如果进行了交换动画，则不允许进行其他动画
        } else if (animation_queue_.front()->type() == AnimationFactory::kAnimationRemove) {  // 爆炸动画，TODO
            int id = ((AnimationRemove *)(animation_queue_.front()))->id();

            if (animation_queue_.front()->tag() == 1) {
                stones_[id].set_removing(Stone::kRemovingSpeed, Stone::kRemovingAcceleration);
            } else {
                stones_[id].set_active(false);
            }

            animation_queue_.pop();

            remove_in_recent_frame_ = true;  // 有remove发生，将记录变量设置为true
        } else if (animation_queue_.front()->type() == AnimationFactory::kAnimationWait) {
            animation_queue_.front()->Update();
            if (animation_queue_.front()->left_frames() == 0) {
                animation_queue_.pop();
            }
            break;  // 如果进行了等待动画，则不允许进行其他动画
        } else {
            assert(false);
        }
    }

    // 下落
    if (is_falling) {
        bool have_fallen = false;
        for (auto &pr : stones_) {
            Stone &stone = pr.second;
            if (stone.is_falling()) {
                stone.UpdateFalling();
                have_fallen = true;
            }
        }
        if (!have_fallen) {
            is_falling = false;
            fall_in_recent_frame_ = true;  // ************
        }
    }

    // 暂停宝石
    for (int i = 1; i <= 3 && !pausing_queue_.empty(); i++) {
        int id = pausing_queue_.front().first;
        bool pause = pausing_queue_.front().second;
        pausing_queue_.pop();

        if (stones_[id].is_active()) {
            stones_[id].set_pausing(pause);
        }
    }

    // std::cout << "Hypercube::StoneManager::Update End" << std::endl;
}

void StoneManager::Draw(QOpenGLShaderProgram &program) {
    if (!have_initialized_) return;
    QMatrix4x4 model;

    std::vector<int> is_not_active_stones_;
    for (auto &pr : stones_) {
        const int &id = pr.first;
        Stone &stone = pr.second;

        if (!stone.is_active()) {
            is_not_active_stones_.push_back(id);
            continue;
        }

        model.setToIdentity();
        model.translate(stone.x(), stone.y(), stone.z());
        model.rotate(stone.angle(), 0.f, 1.f, 0.f);
        model.scale(0.3f);
        program.setUniformValue("model", model);

        if (stone.is_pausing()) program.setUniformValue("onlySpecular", true);

        Model *model = gem_model_manager_.GetModel(stone.type());
        if (model != nullptr) {
            model->Draw(program);
        }

        if (stone.is_pausing()) program.setUniformValue("onlySpecular", false);
    }

    // 清除无用宝石
    for (auto &id : is_not_active_stones_) {
        stones_.erase(id);
    }
}

}  // namespace Hypercube
