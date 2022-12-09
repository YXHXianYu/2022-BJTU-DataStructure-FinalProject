#ifndef HYPERCUBE_STONE_H
#define HYPERCUBE_STONE_H

namespace Hypercube {

// 储存单个宝石的信息
class Stone {
    // 基本函数
   public:
    Stone();
    Stone(int x, int y, int z, int angle, int type);

    int x() const;
    int y() const;
    int z() const;
    float angle() const;
    int type() const;

    bool is_active() const;
    void set_active(bool active);

    bool is_falling() const;
    float falling_speed() const;
    float falling_acceleration() const;
    void set_falling(float acceleration, int falling_target_y);  // 初始化falling相关的三个变量

    bool is_rotating() const;
    float rotating_speed() const;
    void set_rotating_speed(float speed);

    bool is_swaping() const;
    void set_swaping(int target_x, int target_y, float swaping_speed);

    void UpdateRotating();
    void UpdateFalling();
    void UpdateSwaping();

    // 常量
   public:
    static constexpr float kRotatingStatic = 0.0f;
    static constexpr float kRotatingSpeed = 0.4f;
    static constexpr float kRotatingSpeedFast = 4.0f;

    static constexpr float kFallingStatic = 0.0f;
    static constexpr float kFallingSpeed = 0.2f;

    static constexpr int kSwapingStatic = 0.0f;
    static constexpr float kSwapingSpeed = 0.2f;

    // 属性
   private:
    int x_;
    int y_;
    int z_;
    float angle_;
    int type_;

    bool active_;

    float falling_speed_;
    float falling_acceleration_;
    int falling_target_y_;

    float rotating_speed_;

    float swaping_timer_;
    float swaping_speed_;
    int swaping_start_x_;
    int swaping_start_y_;
    int swaping_target_x_;
    int swaping_target_y_;
};

}  // namespace Hypercube

#endif  // HYPERCUBE_STONE_H
