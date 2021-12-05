#pragma once

#include "../Math/Matrix4x4.h"
#include "../Math/Vector.h"

using std::cout;
using std::cerr;
using std::endl;

enum class Space
{
    World,
    Local
};

class Transform
{
public:
    Transform();
    Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

    Vector3 getPosition() const;
    Quaternion getRotation() const;
    Vector3 getScale() const;
    Matrix4x4 getTRS();
    Vector3 getForward() const;
    Vector3 transformDirection(const Vector3& dir) const;
    void setPosition(const Vector3& position);
    void setRotation(const Quaternion& rotation);
    void setScale(const Vector3& scale);
    void translate(const Vector3& v, Space space = Space::World);
    void rotate(const Vector3& radians, Space space = Space::World);
private:
    Vector3 m_Position;
    Quaternion m_Rotation;
    Vector3 m_Scale;
    Matrix4x4 m_TRS;
    bool m_DirtyTRS;
};
