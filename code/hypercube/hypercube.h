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
#include "stonemanager.h"

namespace Hypercube {

class Hypercube : public QOpenGLWidget, public QOpenGLFunctions_4_5_Core {
    Q_OBJECT
   public:
    explicit Hypercube(QWidget* parent = nullptr);
    virtual ~Hypercube() override;

    StoneManager* GetStoneManager();

   protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

    virtual void wheelEvent(QWheelEvent* event) override;

   public:
    StoneManager* stone_manager_;

   private:
    const unsigned int timer_inteval_ = 10;
    QTimer* timer_;

    QOpenGLShaderProgram shader_program_;

    Camera camera_;

   private:
    QVector3D kCameraPosition = QVector3D(400.f, 400.f, 1000.f);
    QVector4D kBackgroundColor = QVector4D(0.2f, 0.3f, 0.3f, 1.0f);

   signals:
};

}  // namespace Hypercube

#endif  // HYPERCUBE_HYPERCUBE_H
