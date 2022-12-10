#include "hypercube.h"

#include <iostream>

#include "DEBUG.h"

namespace Hypercube {

Hypercube::Hypercube(QWidget *parent) : QOpenGLWidget(parent) {
    // thread
    hypercube_thread_ = new HypercubeThread(this);
    connect(hypercube_thread_, &HypercubeThread::timeout, this, &Hypercube::HypercubeThreadSlot);
    hypercube_thread_->start();
    // camera
    camera_.Position = kCameraPosition;
    // msaa
    SetMSAA(16);
}

Hypercube::~Hypercube() {
    // thread (no need)
    // stone manager
    delete stone_manager_;
}

StoneManager *Hypercube::GetStoneManager() {
    if (stone_manager_ == nullptr) {
        std::cerr << "WARNING!!! StoneManager is nullptr" << std::endl;
    }
    return stone_manager_;
}

void Hypercube::Demo() {
    std::cerr << "Demo imported." << std::endl;
    // 0. some vars
    int nx = 8;
    int ny = 8;

    auto Checker = [&](int va) {
        if (va != 0) std::cerr << "Error" << std::endl;
    };

    // 1. initialize
    Checker(GetStoneManager()->Init(nx, ny));

    // 2. generate gemstone
    int tot = 0;
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            Checker(GetStoneManager()->Generate(++tot, i, j, rand() % 8 + 1, rand() % 500 + 100));
        }
    }

    // 3. swap
    Checker(GetStoneManager()->SwapStone(1, 2));
    Checker(GetStoneManager()->SwapStone(3, 4));
    Checker(GetStoneManager()->SwapStone(5, 6));
    Checker(GetStoneManager()->SwapStone(1, 10));
    Checker(GetStoneManager()->SwapStone(12, 12 + 8));

    // 4. remove
    Checker(GetStoneManager()->Remove(3));
    Checker(GetStoneManager()->Remove(4));

    // 5. fall
    Checker(GetStoneManager()->FallTo(1, 50));
    Checker(GetStoneManager()->FallTo(2, 50));
    Checker(GetStoneManager()->FallTo(13, 50));
    Checker(GetStoneManager()->FallTo(14, 50));

    // 6. random fall
    for (int i = 1; i <= 10; i++) {
        int random = 0;
        while (random == 0 || random == 3 || random == 4) random = rand() % (nx + ny) + 1;

        Checker(GetStoneManager()->FallTo(random, 50));
    }
}

void Hypercube::SetBlinnPhong(bool enable) { shader_blinn_phong_ = enable; }

void Hypercube::SetRenderMode(int mode) { shader_render_mode_ = mode; }

void Hypercube::SetLightSource(int source) { shader_light_source_ = source; }

void Hypercube::SetHDRExposure(float exposure) { shader_hdr_exposure_ = exposure; }

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

    shader_program_.setUniformValue("lights[0].type", 1);  // 平行光光源
    shader_program_.setUniformValue("lights[0].ambient", 0.2f, 0.2f, 0.2f);
    shader_program_.setUniformValue("lights[0].diffuse", 0.5f, 0.5f, 0.5f);
    shader_program_.setUniformValue("lights[0].specular", 0.5f, 0.5f, 0.5f);
    shader_program_.setUniformValue("lights[0].direction", 0.0f, 0.5f, -0.3f);

    shader_program_.setUniformValue("lights[1].type", 0);  // 点光源
    shader_program_.setUniformValue("lights[1].ambient", 0.2f, 0.2f, 0.2f);
    shader_program_.setUniformValue("lights[1].diffuse", 0.6f, 0.6f, 0.6f);
    shader_program_.setUniformValue("lights[1].specular", 0.9f, 0.9f, 0.9f);
    shader_program_.setUniformValue("lights[1].position", 400.0f, 900.0f, 300.0f);

    shader_program_.setUniformValue("material.shininess", 32.0f);

    shader_program_.setUniformValue("view", view);
    shader_program_.setUniformValue("projection", projection);

    shader_program_.setUniformValue("onlySpecular", false);

    shader_program_.setUniformValue("enableBlinnPhong", shader_blinn_phong_);
    shader_program_.setUniformValue("renderMode", shader_render_mode_);
    shader_program_.setUniformValue("lightSource", shader_light_source_);
    shader_program_.setUniformValue("hdrExposure", shader_hdr_exposure_);

    if (stone_manager_ != nullptr) stone_manager_->Draw(shader_program_);
}

void Hypercube::resizeGL(int w, int h) {
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void Hypercube::wheelEvent(QWheelEvent *event) { camera_.ProcessMouseScroll(event->angleDelta().y() / 90); }

void Hypercube::SetMSAA(int sampling_multiple) {  // 设置MSAA
    QSurfaceFormat format;
    format.setSamples(sampling_multiple);
    setFormat(format);
}

void Hypercube::HypercubeThreadSlot() {
    GetStoneManager()->Update();
    update();
}

}  // namespace Hypercube
