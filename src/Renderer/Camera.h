#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "../Math/Matrix4x4.h"
#include "Transform.h"

using std::cout;
using std::endl;

class Camera
{
public:
    Camera(const Vector3& pos, const Vector3& rot);
    ~Camera();
    void update();
    Transform* getTransform() const;
    Matrix4x4 getViewMatrix() const;
private:
    Transform* m_Transform;
    Matrix4x4 m_ViewMatrix;
};

#endif // CAMERA_H
