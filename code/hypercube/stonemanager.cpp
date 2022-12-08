#include "stonemanager.h"

namespace Hypercube {

StoneManager::StoneManager(QOpenGLFunctions_4_5_Core *func) : gem_model_manager_(func) {}

int StoneManager::Init(int nx, int ny) {
    nx_ = nx;
    ny_ = ny;

    position_.resize(nx);
    for (int i = 0; i < nx; i++) position_[i].resize(ny);

    stones_.clear();
    stones_.push_back(Stone());  // 占位，确保编号从1开始

    is_playing_animation_ = false;

    while (!swap_queue_.empty()) swap_queue_.pop();

    if (timer != nullptr) {
        if (timer->isActive()) {
            timer->stop();
        }
        delete timer;
    }
    timer = new QTimer();
    connect(timer, &QTimer::timeout, [=]() { Update(); });

    return kSuccess;
}

void StoneManager::Start() { timer->start(10); }

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
    stones_.back().set_falling(Stone::kFallingAcceleration, coordinate_y);

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

    int id = position_[x][y];
    position_[x][y] = 0;       // 清除原位置
    position_[x][tar_y] = id;  // 填入新位置
    stones_[id].set_falling(Stone::kFallingAcceleration, PositionToCoordinateY(y));

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
    swap_queue_.push(std::make_pair(id1, id2));

    return kSuccess;
}

bool StoneManager::isPlayingAnimation() { return is_playing_animation_; }

void StoneManager::Update() {
    is_playing_animation_ = false;
    bool is_falling = false;
    for (int i = 0; i < nx_; i++) {
        for (int j = 0; j < ny_; j++) {
            if (position_[i][j] == 0) continue;

            int id = position_[i][j];
            if (stones_[id].is_falling() == false) continue;

            stones_[id].update();
            is_falling = true;
            is_playing_animation_ = true;
        }
    }

    if (is_falling == false) {
        if (!swap_queue_.empty()) {
            int id1 = swap_queue_.front().first;
            int id2 = swap_queue_.front().second;

            if (is_swaping == false) {
                is_swaping = true;
                stones_[id1].set_swaping(stones_[id2].x(), stones_[id2].y(), Stone::kSwapingSpeed);
                stones_[id2].set_swaping(stones_[id1].x(), stones_[id1].y(), Stone::kSwapingSpeed);
            } else {
                if (stones_[id1].is_swaping() || stones_[id2].is_swaping()) {
                    stones_[id1].update();
                    stones_[id2].update();
                } else {
                    swap_queue_.pop();
                    is_swaping = false;
                }
            }
            is_playing_animation_ = true;
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
            model.scale(0.15f);
            program.setUniformValue("model", model);

            gem_model_manager_.GetModel(stone.type())->Draw(program);
        }
    }
}

}  // namespace Hypercube
