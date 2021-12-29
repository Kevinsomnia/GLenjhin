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
    Transform(const Vector3& localPosition, const Vector3& localRotation, const Vector3& localScale);

    Vector3 localPosition() const { return m_LocalPosition; }
    Vector3 localRotation() const { return m_LocalRotation; }
    Vector3 localScale() const { return m_LocalScale; }
    Matrix4x4 prevWorldTRS() const { return m_PrevWorldTRS; }
    Matrix4x4 worldTRS();
    Vector3 forward();
    Vector3 transformDirection(const Vector3& dir);
    void earlyUpdate();
    void setLocalPosition(const Vector3& position);
    void setLocalRotation(const Vector3& rotation);
    void setLocalScale(const Vector3& scale);
    void translate(const Vector3& v, Space space = Space::World);
    void rotate(const Vector3& r, Space space = Space::World);
private:
    Vector3 m_LocalPosition;
    Vector3 m_LocalRotation;    // Measured in radians
    Vector3 m_LocalScale;
    Matrix4x4 m_PrevWorldTRS;
    Matrix4x4 m_WorldTRS;
    bool m_DirtyTRS;
};
