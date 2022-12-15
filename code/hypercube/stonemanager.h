#ifndef HYPERCUBE_STONEMANAGER_H
#define HYPERCUBE_STONEMANAGER_H

#include <QMatrix4x4>
#include <QObject>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <map>
#include <queue>
#include <vector>

#include "animation/animationfactory.h"
#include "gemmodelmanager.h"
#include "hstone.h"

namespace Hypercube {

// 管理宝石
// - 管理每个宝石的位置
// - 提供操作宝石的方法
// - 处理宝石动画
//   - 动画分为：下落、交换、爆炸
//   - 下落和交换不能并行，爆炸可以并行。动画队列只存放交换。
//   - 如果当前存在下落中的宝石，那么就先等待下落完成；反之，则进行交换动画。
class StoneManager : public QObject {
    Q_OBJECT

   public:
    StoneManager(QOpenGLFunctions_4_5_Core* func);
    virtual ~StoneManager();

    // 初始化
    // - 成功，返回 kSuccess
    // - 若已经初始化过，失败，返回 kFailureHaveInitialized
    int Init(int nx, int ny);

    // 启动（弃用deprecated）
    void Start();

    // 在(x, y)格生成一个编号为id、类型为type的宝石。
    // 下落fallen_pixel像素后到达目标
    // - 成功，返回 kSuccess
    // - 如果该编号的宝石已经生成，失败，返回 kFailureArgumentError
    int Generate(int id, int x, int y, int type, int fallen_pixel = -1);

    // 删除编号为id的宝石。如果playAnimation为真，则播放爆炸动画
    // - 成功，返回 kSuccess
    // - 如果该宝石不存在，失败，返回 kFailureArgumentError
    int Remove(int id, bool playAnimation = false);

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

    // 设置暂停状态
    int SetPause(bool is_pause);

    // 是否暂停
    bool IsPause() const;

    // 获取最近是否有过Remove动画发生
    // - 通过接口获取后，记录变量会重置为false
    bool haveRemoveInRecentFrame();

    // 获取最近是否有过Fall动画落到底
    // - 通过接口获取后，记录变量会重置为false
    bool haveFallInRecentFrame();

    // 刷新动画
    void Update();

    // 绘制
    void Draw(QOpenGLShaderProgram& program);

   public:
    // Generate's arguments
    static constexpr int kSuccess = 0;
    static constexpr int kFailureArgumentError = 1;
    static constexpr int kFailureOccupied = 2;
    static constexpr int kFailureEmpty = 3;
    static constexpr int kFailureHaveInitialized = 4;
    static constexpr int kFailureHaveNotInitialized = 5;
    static constexpr int kFailureIDHaveBeenUsed = 6;
    static constexpr int kFailureIDNotFound = 7;
    static constexpr int kFailureHavePaused = 8;
    static constexpr int kFailureHaveContinued = 9;

    // SetRotate's arguments
    static constexpr int kStatic = 0;
    static constexpr int kRotate = 1;
    static constexpr int kRotateFast = 2;
    static constexpr int kRotateInverse = 3;
    static constexpr int kRotateFastInverse = 4;

   private:
    static constexpr int kDelta = 100;
    static constexpr int kDeltaHalf = kDelta >> 1;
    int PositionToCoordinateX(int x) { return x * kDelta + kDeltaHalf; }
    int PositionToCoordinateY(int y) { return (ny_ - y) * kDelta - kDeltaHalf; }

   private:
    bool have_initialized_;
    int nx_, ny_;
    std::map<int, Stone> stones_;

    std::queue<Animation*> animation_queue_;
    bool is_falling;
    bool remove_in_recent_frame_;
    bool fall_in_recent_frame_;

    bool is_pausing;
    std::queue<std::pair<int, bool>> pausing_queue_;

    GemModelManager gem_model_manager_;
};

}  // namespace Hypercube

#endif  // HYPERCUBE_STONEMANAGER_H
