#ifndef HYPERCUBE_STONEMANAGER_H
#define HYPERCUBE_STONEMANAGER_H

#include <QMatrix4x4>
#include <QObject>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <queue>
#include <vector>

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

    // 启动
    void Start();

    // 在(x, y)格生成一个type类的宝石。下落fallen_height像素后到达目标，下落速度为fallen_speed
    // - 成功，返回 kSuccess
    // - 如果参数错误，失败，返回 kFailureArgumentError
    // - 如果(x, y)已经存在一个宝石，失败，返回 kFailureOccupied
    int Generate(int x, int y, int type, int fallen_pixel = -1);

    // 删除(x, y)格的宝石
    // - 成功，返回 kSuccess
    // - 如果参数错误，失败，返回 kFailureArgumentError
    // - 如果(x, y)为空，失败，返回 kFailureEmpty
    int Remove(int x, int y);

    // 设置是否旋转
    // - 成功，返回 kSuccess
    int SetRotate(int x, int y, int rotateMode);

    // 让(x, y)的宝石下落到(x, tar_y)处
    // - 包含两个动作：下落 & 移动
    // - 成功，返回 kSuccess
    int FallTo(int x, int y, int tar_y);

    // 交换位于(x1, y1)的宝石和位于(x2, y2)的宝石
    // - 成功，返回 kSuccess
    int SwapStone(int x1, int y1, int x2, int y2);

    // 是否正在播放动画
    bool isPlayingAnimation();

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
    int nx_, ny_;  // x轴方向和y轴方向
    std::vector<std::vector<int>> position_;
    std::vector<Stone> stones_;

    bool is_playing_animation_;
    std::queue<std::pair<int, int>> animation_queue_;
    bool is_swaping_;
    QTimer* timer_;

    GemModelManager gem_model_manager_;
};

}  // namespace Hypercube

#endif  // HYPERCUBE_STONEMANAGER_H
