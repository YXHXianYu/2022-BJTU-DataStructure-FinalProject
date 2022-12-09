#include "hypercube.h"

#include <iostream>

#include "DEBUG.h"

namespace Hypercube {

Hypercube::Hypercube(QWidget *parent) : QOpenGLWidget(parent) {
    // timer
    hypercube_thread_ = new HypercubeThread(this);
    connect(hypercube_thread_, &HypercubeThread::timeout, this, &Hypercube::HypercubeThreadSlot);
    hypercube_thread_->start();
    // camera
    camera_.Position = kCameraPosition;
}

Hypercube::~Hypercube() {
    // thread (no need)

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
    if (DEBUG) std::cerr << "Hypercube::Hypercube::initializeGL - 0" << std::endl;
    // GL Functions
    initializeOpenGLFunctions();
    if (DEBUG) std::cerr << "Hypercube::Hypercube::initializeGL - 1" << std::endl;
    // Stone Manager
    stone_manager_ = new StoneManager(QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_5_Core>());
    if (DEBUG) std::cerr << "Hypercube::Hypercube::initializeGL - 2" << std::endl;
    // Shader Program
    shader_program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/common.vert");
    shader_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/common.frag");
    bool success = shader_program_.link();
    if (!success) qDebug() << "Hypercube::Hypercube::InitializeGL Error: " << shader_program_.log();
    if (DEBUG) std::cerr << "Hypercube::Hypercube::initializeGL - 3 END" << std::endl;
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

    shader_program_.setUniformValue("numberOfLights", 2);

    shader_program_.setUniformValue("lights[0].type", 1);
    shader_program_.setUniformValue("lights[0].ambient", 0.3f, 0.3f, 0.3f);
    shader_program_.setUniformValue("lights[0].diffuse", 0.7f, 0.7f, 0.7f);
    shader_program_.setUniformValue("lights[0].specular", 1.0f, 1.0f, 1.0f);
    shader_program_.setUniformValue("lights[0].direction", 0.0f, 0.5f, -0.3f);

    shader_program_.setUniformValue("lights[1].type", 0);  // 点光源
    shader_program_.setUniformValue("lights[1].ambient", 0.1f, 0.1f, 0.1f);
    shader_program_.setUniformValue("lights[1].diffuse", 0.5f, 0.5f, 0.5f);
    shader_program_.setUniformValue("lights[1].specular", 1.0f, 1.0f, 1.0f);
    shader_program_.setUniformValue("lights[1].position", 400.0f, 900.0f, 400.0f);

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

void Hypercube::HypercubeThreadSlot() {
    // std::cerr << "slot" << std::endl;
    if (stone_manager_ != nullptr) {
        stone_manager_->Update();
    }
    update();
}

}  // namespace Hypercube
