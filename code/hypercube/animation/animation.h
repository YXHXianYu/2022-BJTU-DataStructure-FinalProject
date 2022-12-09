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

    int tag() const;
    void set_tag(int tag);

    int tag_id() const;
    void set_tag_id(int tag_id);

   protected:
    int type_;
    int total_frames_;
    int left_frames_;
    int tag_;     // for you, do anything you want
    int tag_id_;  // for you, do anything you want
};

}  // namespace Hypercube

#endif  // ANIMATION_H
