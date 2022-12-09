# Hybercube - 3D&动画模块

#### 1. 简介

* Hypercube是什么？

  * 本项目的3D与动画模块
  * 一个极其基础的渲染器

* Hypercube能做什么？

  * **在你的宝石迷阵游戏中，实现3D效果**
  * 经过修改后，你可以用它渲染一些给定的3D模型
  * 作为你学习QtOpenGL的素材

* 我要如何使用Hypercube？

  * 可以参照本项目中的代码中的代码。
    * GameWindow类的构造函数，其中演示了如何初始化一个Hypercube对象
    * ./code/core文件夹下的代码，其中演示了如何使用Hypercube对象
  * 也可以参照Hypercube模块中自带的 **Demo**。

    * hypercube.h下的Demo函数
  * Hypercube导入模型时，使用了 **Assimp库**。所以你也需要在你的项目中导入Assimp库。
    * 可以参照本项目的qmake文件(.pro)配置第三方库。
    * 本项目中，Assimp库的编译环境为：Qt 5.9.9 MinGW 32bit（mingw版本为5.3.0）

* 具体的使用流程

  1. 初始化Hypercube

     ```c++
     hypercube_ = new Hypercube::Hypercube(this);
     ```

  2. 设置Hypercube窗体的位置

     ```c++
     hypercube_->setFixedSize(hypercube_size.x(), hypercube_size.y());
     hypercube_->setGeometry(opengl_up_left.x(), opengl_up_left.y(), hypercube_->width(), hypercube_->height());
     // 因为Hypercube是QOpenGLWidget的子类
     // 所以你可以通过Qt控制窗体的函数来设置Hypercube的位置
     ```

  3. 开始使用Hypercube的接口吧！

     ```c++
     // Demo
     // 本段代码中，hypercube指向一个已经初始化完毕的Hypercube对象
     // 0. some vars
     int nx = 8;
     int ny = 8;
     
     auto Checker = [&](int va) {
         if (va != 0) std::cerr << "Error" << std::endl;
     };
     
     // 1. initialize
     Checker(hypercube->GetStoneManager()->Init(nx, ny));
     
     // 2. generate gemstone
     int tot = 0;
     for (int i = 0; i < nx; i++) {
         for (int j = 0; j < ny; j++) {
             Checker(hypercube->GetStoneManager()->Generate(++tot, i, j, rand() % 8 + 1, rand() % 500 + 100));
         }
     }
     
     // 3. swap
     Checker(hypercube->GetStoneManager()->SwapStone(1, 2));
     Checker(hypercube->GetStoneManager()->SwapStone(3, 4));
     Checker(hypercube->GetStoneManager()->SwapStone(5, 6));
     Checker(hypercube->GetStoneManager()->SwapStone(1, 10));
     Checker(hypercube->GetStoneManager()->SwapStone(12, 12 + 8));
     
     // 4. remove
     Checker(hypercube->GetStoneManager()->Remove(3));
     Checker(hypercube->GetStoneManager()->Remove(4));
     
     // 5. fall
     Checker(hypercube->GetStoneManager()->FallTo(1, 50));
     Checker(hypercube->GetStoneManager()->FallTo(2, 50));
     Checker(hypercube->GetStoneManager()->FallTo(13, 50));
     Checker(hypercube->GetStoneManager()->FallTo(14, 50));
     
     // 6. random fall
     for (int i = 1; i <= 10; i++) {
         int random = 0;
         while (random == 0 || random == 3 || random == 4) random = rand() % (nx + ny) + 1;
     
         Checker(hypercube->GetStoneManager()->FallTo(random, 50));
     }
     ```

* 注意！

  * **因为QtOpenGL封装的原因，所以必须在Hypercube窗体出现后0.5s才能调用GetStoneManager**
    * 可以参照GameWindow类的构造函数，其中在500ms后才调用Demo
  * 否则会出现崩溃的错误（访问空指针、非法内存）

#### 2. 渲染器特性

* Phong光照模型与Blinn-Phong光照模型
* 多光源渲染
* 【增加中...】


#### 3. 接口

* 在Hypercube初始化完毕后，可以调用Hypercube对象的GetStoneManager方法。通过该方法，你可以往模块中导入宝石数据，让Hypercube帮你完成动画，并在窗体中渲染出3D图像。

* StoneManager类给你提供了许多方法，例如：生成、删除、旋转、交换、下落。你可以通过自己设定的编号来管理每个宝石

* 接口中的方法：

  ```c++
  // 初始化
  // - 成功，返回 kSuccess
  // - 若已经初始化过，失败，返回 kFailureHaveInitialized
  int Init(int nx, int ny);
  
  // 在(x, y)格生成一个编号为id、类型为type的宝石。
  // 下落fallen_pixel像素后到达目标
  // - 成功，返回 kSuccess
  // - 如果该编号的宝石已经生成，失败，返回 kFailureArgumentError
  int Generate(int id, int x, int y, int type, int fallen_pixel = -1);
  
  // 删除编号为id的宝石
  // - 成功，返回 kSuccess
  // - 如果该宝石不存在，失败，返回 kFailureArgumentError
  int Remove(int id);
  
  // 设置编号为id的宝石的旋转模式
  // - 成功，返回 kSuccess
  // - 如果该宝石不存在，失败，返回 kFailureArgumentError
  int SetRotate(int id, int rotateMode);
  
  // 让编号为id的宝石下落到(x, tar_y)处
  // - 成功，返回 kSuccess
  // - 如果该宝石不存在，失败，返回 kFailureArgumentError
  int FallTo(int id, int tar_y);
  
  // 交换编号为id1的宝石和编号为id2的宝石
  // - 成功，返回 kSuccess
  // - 如果宝石不存在，失败，返回 kFailureArgumentError
  int SwapStone(int id1, int id2);
  
  // 是否正在播放动画
  bool isPlayingAnimation();
  ```

* 接口中的常量

  ```c++
  // Generate's arguments
  static constexpr int kSuccess = 0;
  static constexpr int kFailureArgumentError = 1;
  static constexpr int kFailureOccupied = 2;
  static constexpr int kFailureEmpty = 3;
  static constexpr int kFailureHaveInitialized = 4;
  static constexpr int kFailureHaveNotInitialized = 5;
  static constexpr int kFailureIDHaveBeenUsed = 6;
  static constexpr int kFailureIDNotFound = 7;
  
  // SetRotate's arguments
  static constexpr int kStatic = 0;
  static constexpr int kRotate = 1;
  static constexpr int kRotateFast = 2;
  static constexpr int kRotateInverse = 3;
  static constexpr int kRotateFastInverse = 4;
  ```

#### 4. 实现原理

* Hypercube可以分为两大部分：3D和动画
  * 3D部分：Hypercube类、GemModelManager类、Model类、Mesh类、Camera类
  * 动画部分：StoneManager类、Stone类、Animation类
* Hypercube类
  * 是QOpenGLWidget的子类
  * 是一个窗体，可以在上面绘制3D图像
  * 拥有一个线程，这个线程控制模块内的帧数（默认每秒100帧）
* StoneManager类
  * 管理所有的宝石
  * 操作宝石，实现动画
* GemModelManager类
  * 载入模型文件
* 用户通过StoneManager向库发送信息，库收到信息后，会记录下信息（比如将动画填充进animation_queue_队列）
* StoneManager类每帧会对所有宝石的信息进行更新
* Hypercube类每帧会重新渲染一次所有宝石