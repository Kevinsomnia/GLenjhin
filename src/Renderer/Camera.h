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
    Camera(const Vector3& pos, const Vector3& rot, float fieldOfView, float nearClip, float farClip);
    ~Camera();
    void update();
    Transform* getTransform() const;
    Matrix4x4 getViewProjMatrix() const;

    // X = near clip
    // Y = far clip
    // Z = far - near
    // W = 2.0 * near * far
    Vector4 getProjectionParams() const;
private:
    Transform* m_Transform;
    Matrix4x4 m_ViewProjMatrix;
    Vector4 m_ProjectionParams;
    float m_FieldOfView;
    float m_NearClip;
    float m_FarClip;
};

#endif // CAMERA_H
