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
    Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale);

    Vector3 getPosition() const { return m_Position; }
    Vector3 getRotation() const { return m_Rotation; }
    Vector3 getScale() const { return m_Scale; }
    Matrix4x4 getPrevTRS() const { return m_PrevTRS; }
    Matrix4x4 getTRS();
    Vector3 getForward();
    Vector3 transformDirection(const Vector3& dir);
    void earlyUpdate();
    void setPosition(const Vector3& position);
    void setRotation(const Vector3& rotation);
    void setScale(const Vector3& scale);
    void translate(const Vector3& v, Space space = Space::World);
    void rotate(const Vector3& r, Space space = Space::World);
private:
    Vector3 m_Position;
    Vector3 m_Rotation; // Measured in radians
    Vector3 m_Scale;
    Matrix4x4 m_PrevTRS;
    Matrix4x4 m_TRS;
    bool m_DirtyTRS;
};
