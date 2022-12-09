#ifndef ANIMATION_H
#define ANIMATION_H

namespace Hypercube {

// 动画类
// -
class Animation {
   public:
    Animation(int type, int left_frames);

    int type() const;
    int total_frames() const;
    int left_frames() const;
    void Update();

   protected:
    int type_;
    int total_frames_;
    int left_frames_;
};

}  // namespace Hypercube

#endif  // ANIMATION_H
