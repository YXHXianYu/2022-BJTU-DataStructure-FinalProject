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

    bool is_pausing() const;
    void set_pausing(int pausing);

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

    bool is_removing() const;
    void set_removing(int removing_speed, int removing_acceleration);

    void UpdateRotating();
    void UpdateFalling();
    void UpdateSwaping();
    void UpdataRemoving();

    // 常量
   public:
    static constexpr float kRotatingStatic = 0.0f;
    static constexpr float kRotatingSpeed = 0.4f;
    static constexpr float kRotatingSpeedFast = 4.0f;

    static constexpr float kFallingStatic = 0.0f;
    static constexpr float kFallingSpeed = 12.0f;
    static constexpr float kFallingSpeedRandom = -1.0f;

    static constexpr int kSwapingStatic = 0.0f;
    static constexpr float kSwapingSpeed = 10.f;

    static constexpr float kRemovingSpeed = 3.f;
    static constexpr float kRemovingAcceleration = -0.1f;

   private:
    static constexpr int kRemovingEndY = -100;

   private:  // 属性
    int x_;
    int y_;
    int z_;
    float angle_;
    int type_;

    bool pausing_;

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

    float removing_speed_;
    float removing_acceleration_;
};

}  // namespace Hypercube

#endif  // HYPERCUBE_STONE_H
