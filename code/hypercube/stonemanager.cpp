#include "stonemanager.h"

#include <iostream>
#include <random>

namespace Hypercube {

StoneManager::StoneManager(QOpenGLFunctions_4_5_Core *func) : have_initialized_(false), gem_model_manager_(func) {}

StoneManager::~StoneManager() {
    // timer_ (no need)
}

int StoneManager::Init(int nx, int ny) {
    if (have_initialized_) return kFailureHaveInitialized;

    int debug = 1;
    if (debug) std::cerr << "1.0" << std::endl;

    nx_ = nx;
    ny_ = ny;

    if (debug) std::cerr << "1.1" << std::endl;

    position_.resize(nx);
    for (int i = 0; i < nx; i++) position_[i].resize(ny);

    if (debug) std::cerr << "1.2" << std::endl;

    stones_.clear();
    stones_.push_back(Stone());  // 占位，确保编号从1开始

    if (debug) std::cerr << "1.3" << std::endl;

    is_playing_animation_ = false;

    while (!animation_queue_.empty()) animation_queue_.pop();

    if (debug) std::cerr << "1.4" << std::endl;

    if (debug) std::cerr << "1.5" << std::endl;

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, [=]() { Update(); });

    if (debug) std::cerr << "1.6" << std::endl;

    return kSuccess;
}

void StoneManager::Start() { timer_->start(10); }

int StoneManager::Generate(int x, int y, int type, int fallen_pixel) {
    if (x < 0 || x >= nx_ || y < 0 || y >= ny_) {  // 失败，参数越界
        return kFailureArgumentError;
    }
    if (position_[x][y] != 0) {  // 失败，该位置已被占用
        return kFailureOccupied;
    }

    int coordinate_x = PositionToCoordinateX(x);
    int coordinate_y = PositionToCoordinateY(y);
    int coordinate_start_y = (fallen_pixel != -1 ? coordinate_y + fallen_pixel : PositionToCoordinateY(-1));

    stones_.push_back(Stone(coordinate_x, coordinate_start_y, 0, 0, type));
    stones_.back().set_rotating_speed(Stone::kRotatingSpeed);
    // stones_.back().set_falling(Stone::kFallingSpeed, coordinate_y);
    stones_.back().set_falling((rand() % 10 + 1) / 5.f, coordinate_y);

    animation_queue_.push(std::make_pair(-1, -1));

    position_[x][y] = stones_.size() - 1;

    return kSuccess;
}

int StoneManager::Remove(int x, int y) {
    if (x < 0 || x >= nx_ || y < 0 || y >= ny_) {  // 失败，参数越界
        return kFailureArgumentError;
    }
    if (position_[x][y] == 0) {  // 失败，该位置无宝石
        return kFailureEmpty;
    }

    // int id = position_[x][y];
    // play boom animation
    position_[x][y] = 0;

    return kSuccess;
}

int StoneManager::SetRotate(int x, int y, int rotateMode) {
    if (x < 0 || x >= nx_ || y < 0 || y >= ny_) {  // 失败，参数越界
        return kFailureArgumentError;
    }
    if (position_[x][y] == 0) {  // 失败，该位置无宝石
        return kFailureEmpty;
    }

    int id = position_[x][y];
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

int StoneManager::FallTo(int x, int y, int tar_y) {
    if (x < 0 || x >= nx_ || y < 0 || y >= ny_ || tar_y < 0 || tar_y >= ny_) {  // 失败，参数越界
        return kFailureArgumentError;
    }
    if (position_[x][y] == 0) {  // 失败，该位置无宝石
        return kFailureEmpty;
    }
    if (position_[x][tar_y] != 0) {  // 失败，目标位置被占用
        return kFailureOccupied;
    }

    std::cerr << "(" << x << ", " << y << ") will fall" << std::endl;

    int id = position_[x][y];
    position_[x][y] = 0;       // 清除原位置
    position_[x][tar_y] = id;  // 填入新位置
    stones_[id].set_falling(Stone::kFallingSpeed, PositionToCoordinateY(tar_y));

    animation_queue_.push(std::make_pair(-1, -1));

    return kSuccess;
}

int StoneManager::SwapStone(int x1, int y1, int x2, int y2) {
    if (x1 < 0 || x1 >= nx_ || y1 < 0 || y1 >= ny_) {  // 失败，参数越界
        return kFailureArgumentError;
    }
    if (x2 < 0 || x2 >= nx_ || y2 < 0 || y2 >= ny_) {  // 失败，参数越界
        return kFailureArgumentError;
    }
    if (position_[x1][y1] == 0) {  // 失败，该位置无宝石
        return kFailureEmpty;
    }
    if (position_[x2][y2] == 0) {  // 失败，该位置无宝石
        return kFailureEmpty;
    }

    int id1 = position_[x1][y1];
    int id2 = position_[x2][y2];
    std::swap(position_[x1][y1], position_[x2][y2]);
    animation_queue_.push(std::make_pair(id1, id2));

    return kSuccess;
}

bool StoneManager::isPlayingAnimation() { return animation_queue_.size() > 0; }

void StoneManager::Update() {
    // std::cout << "Hypercube::StoneManager::Update" << std::endl;

    for (int i = 0; i < nx_; i++) {
        for (int j = 0; j < ny_; j++) {
            if (position_[i][j] == 0) continue;
            int id = position_[i][j];
            stones_[id].UpdateRotating();
        }
    }

    if (!animation_queue_.empty() && animation_queue_.front().first == -1 && animation_queue_.front().second == -1) {
        // falling
        bool is_falling = false;
        for (int i = 0; i < nx_; i++) {
            for (int j = 0; j < ny_; j++) {
                if (position_[i][j] == 0) continue;
                int id = position_[i][j];
                if (stones_[id].is_falling()) {
                    stones_[id].UpdateFalling();
                    is_falling = true;
                }
            }
        }
        if (!is_falling) {
            while (!animation_queue_.empty() && animation_queue_.front().first == -1 && animation_queue_.front().second == -1) {
                animation_queue_.pop();
            }
        }
    } else if (!animation_queue_.empty()) {
        // swaping
        int id1 = animation_queue_.front().first;
        int id2 = animation_queue_.front().second;

        if (is_swaping_ == false) {
            is_swaping_ = true;
            stones_[id1].set_swaping(stones_[id2].x(), stones_[id2].y(), Stone::kSwapingSpeed);
            stones_[id2].set_swaping(stones_[id1].x(), stones_[id1].y(), Stone::kSwapingSpeed);
        } else {
            if (stones_[id1].is_swaping() || stones_[id2].is_swaping()) {
                stones_[id1].UpdateSwaping();
                stones_[id2].UpdateSwaping();
            } else {
                animation_queue_.pop();
                is_swaping_ = false;
            }
        }
    }
}

void StoneManager::Draw(QOpenGLShaderProgram &program) {
    QMatrix4x4 model;
    for (int i = 0; i < nx_; i++) {
        for (int j = 0; j < ny_; j++) {
            if (position_[i][j] == 0) continue;

            Stone &stone = stones_[position_[i][j]];
            model.setToIdentity();
            model.translate(stone.x(), stone.y(), stone.z());
            model.rotate(stone.angle(), 0.f, 1.f, 0.f);
            model.scale(0.3f);
            program.setUniformValue("model", model);

            Model *model = gem_model_manager_.GetModel(stone.type());
            if (model != nullptr) {
                model->Draw(program);
            }
        }
    }
}

}  // namespace Hypercube
