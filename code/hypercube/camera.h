#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <vector>

namespace Hypercube {

// 移动方向枚举量. 是一种抽象，以避开特定于窗口系统的输入方法
// 我们这里是WSAD
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

// 默认值
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// 一个抽象的camera类，用于处理输入并计算相应的Euler角度、向量和矩阵，以便在OpenGL中使用
class Camera {
   public:
    // camera Attributes
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), float yaw = YAW,
           float pitch = PITCH)
        : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = QVector3D(posX, posY, posZ);
        WorldUp = QVector3D(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    QMatrix4x4 GetViewMatrix() {
        QMatrix4x4 theMatrix;
        theMatrix.lookAt(Position, Position + Front, Up);
        return theMatrix;
    }

    // 处理从任何类似键盘的输入系统接收的输入。接受摄像机定义枚举形式的输入参数（从窗口系统中提取）
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD) Position += Front * velocity;
        if (direction == BACKWARD) Position -= Front * velocity;
        if (direction == LEFT) Position -= Right * velocity;
        if (direction == RIGHT) Position += Right * velocity;
        if (direction == DOWN) Position -= Up * velocity;
        if (direction == UP) Position += Up * velocity;
    }

    // 处理从鼠标输入系统接收的输入。需要x和y方向上的偏移值。
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // 确保当投球超出边界时，屏幕不会翻转
        if (constrainPitch) {
            if (Pitch > 89.0f) Pitch = 89.0f;
            if (Pitch < -89.0f) Pitch = -89.0f;
        }

        // 使用更新的Euler角度更新前、右和上矢量
        updateCameraVectors();
    }

    // 处理从鼠标滚轮事件接收的输入。仅需要在垂直车轮轴上输入
    void ProcessMouseScroll(float yoffset) {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f) Zoom = 1.0f;
        if (Zoom > 75.0f) Zoom = 75.0f;
    }

    QVector3D GetPosition() { return Position; }

   private:
    // 根据相机的（更新的）Euler角度计算前矢量
    void updateCameraVectors() {
        // calculate the new Front vector
        float PI = 3.1415926;
        QVector3D front;
        front.setX(cos(Yaw * PI / 180.0) * cos(Pitch * PI / 180.0));
        front.setY(sin(Pitch * PI / 180.0));
        front.setZ(sin(Yaw * PI / 180.0) * cos(Pitch * PI / 180.0));
        front.normalize();
        Front = front;
        // also re-calculate the Right and Up vector
        Right = QVector3D::crossProduct(Front, WorldUp);
        // 标准化向量，因为向上或向下看得越多，向量的长度就越接近0，这会导致移动速度变慢。
        Right.normalize();
        Up = QVector3D::crossProduct(Right, Front);
        Up.normalize();
    }
};
}  // namespace Hypercube
#endif
