#ifndef HYPERCUBE_HYPERCUBE_H
#define HYPERCUBE_HYPERCUBE_H

#include <QMatrix4x4>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>
#include <QVector3D>
#include <QVector4D>
#include <QWheelEvent>

#include "camera.h"
#include "hypercubethread.h"
#include "stonemanager.h"

namespace Hypercube {

class Hypercube : public QOpenGLWidget, public QOpenGLFunctions_4_5_Core {
    Q_OBJECT
   public:
    explicit Hypercube(QWidget* parent = nullptr);
    virtual ~Hypercube() override;

    StoneManager* GetStoneManager();

    void Demo();

    void SetBlinnPhong(bool enable);      // 设置是否开启BlinnPhong光照模型
    void SetRenderMode(int mode);         // 设置渲染模式
    void SetLightSource(int source);      // 设置光源
    void SetHDRExposure(float exposure);  // 设置HDR曝光度

   protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void wheelEvent(QWheelEvent* event) override;

   private:
    void SetMSAA(int sampling_multiple);

   private:
    const unsigned int timer_inteval_ = 10;

    StoneManager* stone_manager_;        // StoneManager
    HypercubeThread* hypercube_thread_;  // 控制帧数的线程
    Camera camera_;                      // 摄像机

    QOpenGLShaderProgram shader_program_;      // 渲染程序
    QOpenGLShaderProgram shader_toy_program_;  // shadertoy渲染程序
    Model* background;
    QTime start_time;

    // 一些参数
    QVector3D kCameraPosition = QVector3D(400.f, 400.f, 1000.f);     // 摄像机位置
    QVector4D kBackgroundColor = QVector4D(0.2f, 0.3f, 0.3f, 1.0f);  // 背景颜色

    // 着色器设置
    bool shader_blinn_phong_ = true;
    int shader_render_mode_ = 0;
    int shader_light_source_ = 0;
    float shader_hdr_exposure_ = 0.0f;

   public slots:
    void HypercubeThreadSlot();

   signals:
};

}  // namespace Hypercube

#endif  // HYPERCUBE_HYPERCUBE_H
