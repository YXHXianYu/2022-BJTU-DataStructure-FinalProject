#include "hstone.h"

#include <cassert>
#include <cmath>
#include <iostream>

namespace Hypercube {

Stone::Stone() {}

Stone::Stone(int x, int y, int z, int angle, int type) : x_(x), y_(y), z_(z), angle_(angle), type_(type), active_(true) {
    falling_target_y_ = 0;
    swaping_timer_ = 0;
    rotating_speed_ = 0;
}

int Stone::x() const { return x_; }
int Stone::y() const { return y_; }
int Stone::z() const { return z_; }
float Stone::angle() const { return angle_; }
int Stone::type() const { return type_; }

bool Stone::is_active() const { return active_; }
void Stone::set_active(bool active) { active_ = active; }

bool Stone::is_falling() const { return falling_target_y_ != 0; }
float Stone::falling_speed() const { return falling_speed_; }
float Stone::falling_acceleration() const { return falling_acceleration_; }
void Stone::set_falling(float speed, int falling_target_y) {
    if (std::abs(speed - kFallingSpeedRandom) <= 1e-6) {
        speed = rand() % 4 + 4;
    }
    falling_speed_ = speed;
    falling_target_y_ = falling_target_y;
}

bool Stone::is_rotating() const { return rotating_speed_ != 0; }
float Stone::rotating_speed() const { return rotating_speed_; }
void Stone::set_rotating_speed(float speed) { rotating_speed_ = speed; }

bool Stone::is_swaping() const { return swaping_timer_ > 0; }
void Stone::set_swaping(int target_x, int target_y, float swaping_speed) {
    swaping_timer_ = 180;
    swaping_speed_ = swaping_speed;
    swaping_start_x_ = x_;
    swaping_start_y_ = y_;
    swaping_target_x_ = target_x;
    swaping_target_y_ = target_y;
    std::cerr << "set swap: "
              << "(" << x_ << ", " << y_ << ") -> (" << target_x << ", " << target_y << ")" << std::endl;
}

void Stone::UpdateRotating() {
    // rotating
    if (is_rotating()) {
        angle_ += rotating_speed_;
    }
}

void Stone::UpdateFalling() {
    // falling
    if (is_falling()) {
        // falling_speed_ += falling_acceleration_;
        y_ -= falling_speed_;
        if (y_ < falling_target_y_) {
            y_ = falling_target_y_;
            falling_speed_ = 0;
            falling_acceleration_ = 0;
            falling_target_y_ = 0;
        }
    }
}

void Stone::UpdateSwaping() {
    // swaping
    if (is_swaping()) {
        swaping_timer_ -= swaping_speed_;

        if (swaping_timer_ <= 0) {
            x_ = swaping_target_x_;
            y_ = swaping_target_y_;
            z_ = 0;

            swaping_timer_ = 0;
            swaping_speed_ = 0;
            swaping_start_x_ = 0;
            swaping_start_y_ = 0;
            swaping_target_x_ = 0;
            swaping_target_y_ = 0;
        } else {
            const int& ed_x = swaping_start_x_;
            const int& ed_y = swaping_start_y_;
            const int& st_x = swaping_target_x_;
            const int& st_y = swaping_target_y_;
            const int st_z = 0;
            const int ed_z = 0;
            const float& theta = swaping_timer_ / 180.0 * 3.1415926;

            if (st_y == ed_y) {  // alone Ox
                float r = (st_x - ed_x) / 2.f;
                x_ = (st_x + ed_x) / 2.f + r * std::cos(theta);
                y_ = st_y;
                z_ = (st_z + ed_z) / 2.f + r * std::sin(theta);
            } else if (st_x == ed_x) {  // alone Oy
                float r = (st_y - ed_y) / 2.f;
                x_ = st_x;
                y_ = (st_y + ed_y) / 2.f + r * std::cos(theta);
                z_ = (st_z + ed_z) / 2.f + r * std::sin(theta);
            } else {
                assert(false);
            }
        }
    }
}

}  // namespace Hypercube
