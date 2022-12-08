# 3D系统 - Hybercube

#### 0. 系统名称

* Hybercube

#### 1. 接口

* int Generate(int type, int x, int y, int start_y = -1, int fallen_speed = -1)
  * **在某个格子上生成宝石**（宝石会下落至目标位置）
    * 宝石从屏幕上方下落至目标位置（速度随机）
    * 宝石可能会自己旋转（视觉效果）
* int Remove(int x, int y)
  * **清除某个格子上的宝石**（宝石会爆炸）
* int SetRotate(int x, int y, bool isRotate)
  * 让宝石加速旋转（可以用作选择的效果）
* int SwapPos(int x1, int y1, int x2, int y2)
  * 交换两个宝石的效果
* int Start(int nx, int ny)
  * 初始化&启动
* bool IsPlayingAnimation()
  * 是否正在播放动画