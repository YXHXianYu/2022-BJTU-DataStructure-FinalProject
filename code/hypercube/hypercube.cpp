#include "hypercube.h"

#include <iostream>

namespace Hypercube {

Hypercube::Hypercube(QWidget *parent) : QOpenGLWidget(parent) {
    // timer
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, [=]() {
        if (stone_manager_ != nullptr) {
            stone_manager_->Update();
        }
        update();
    });
    timer_->start(timer_inteval_);
    // camera
    camera_.Position = kCameraPosition;
}

Hypercube::~Hypercube() {
    // timer (no need)

    // stone manager
    delete stone_manager_;

    makeCurrent();
    doneCurrent();
}

StoneManager *Hypercube::GetStoneManager() {
    if (stone_manager_ == nullptr) {
        std::cerr << "WARNING!!! StoneManager is nullptr" << std::endl;
    }
    return stone_manager_;
}

void Hypercube::initializeGL() {
    // GL Functions
    initializeOpenGLFunctions();
    // Stone Manager
    stone_manager_ = new StoneManager(QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_5_Core>());
    // Shader Program
    shader_program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/common.vert");
    shader_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/common.frag");
    bool success = shader_program_.link();
    if (!success) qDebug() << "Hypercube::Hypercube::InitializeGL Error: " << shader_program_.log();
}

void Hypercube::paintGL() {
    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;

    view = camera_.GetViewMatrix();
    projection.perspective(camera_.Zoom, float(width() / height()), 0.1f, 2000.f);

    glClearColor(kBackgroundColor.x(), kBackgroundColor.y(), kBackgroundColor.z(), kBackgroundColor.w());
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // shader program
    shader_program_.bind();
    shader_program_.setUniformValue("viewPos", camera_.GetPosition());
    shader_program_.setUniformValue("light.ambient", 0.4f, 0.4f, 0.4f);
    shader_program_.setUniformValue("light.diffuse", 0.9f, 0.9f, 0.9f);
    shader_program_.setUniformValue("light.specular", 1.0f, 1.0f, 1.0f);
    shader_program_.setUniformValue("light.direction", -0.0f, -0.0f, -0.3f);
    shader_program_.setUniformValue("material.shininess", 32.0f);

    shader_program_.setUniformValue("view", view);
    shader_program_.setUniformValue("projection", projection);

    if (stone_manager_ != nullptr) stone_manager_->Draw(shader_program_);
}

void Hypercube::resizeGL(int w, int h) {
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void Hypercube::wheelEvent(QWheelEvent *event) { camera_.ProcessMouseScroll(event->angleDelta().y() / 90); }

}  // namespace Hypercube
