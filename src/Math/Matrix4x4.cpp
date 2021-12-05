#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
    memset(values, 0, sizeof(values));
}

Matrix4x4::Matrix4x4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3)
{
    setRow(0, row0);
    setRow(1, row1);
    setRow(2, row2);
    setRow(3, row3);
}

Matrix4x4::operator const float*() const
{
    return values;
}

Matrix4x4 Matrix4x4::operator *(const Matrix4x4& other) const
{
    Vector4 a0 = getRow(0);
    Vector4 a1 = getRow(1);
    Vector4 a2 = getRow(2);
    Vector4 a3 = getRow(3);
    Vector4 b0 = other.getColumn(0);
    Vector4 b1 = other.getColumn(1);
    Vector4 b2 = other.getColumn(2);
    Vector4 b3 = other.getColumn(3);

    return Matrix4x4(
        Vector4(
            Vector4::Dot(a0, b0),
            Vector4::Dot(a0, b1),
            Vector4::Dot(a0, b2),
            Vector4::Dot(a0, b3)
        ),
        Vector4(
            Vector4::Dot(a1, b0),
            Vector4::Dot(a1, b1),
            Vector4::Dot(a1, b2),
            Vector4::Dot(a1, b3)
        ),
        Vector4(
            Vector4::Dot(a2, b0),
            Vector4::Dot(a2, b1),
            Vector4::Dot(a2, b2),
            Vector4::Dot(a2, b3)
        ),
        Vector4(
            Vector4::Dot(a3, b0),
            Vector4::Dot(a3, b1),
            Vector4::Dot(a3, b2),
            Vector4::Dot(a3, b3)
        )
    );
}

Matrix4x4 Matrix4x4::identity = Matrix4x4(
    Vector4(1.0f, 0.0f, 0.0f, 0.0f),
    Vector4(0.0f, 1.0f, 0.0f, 0.0f),
    Vector4(0.0f, 0.0f, 1.0f, 0.0f),
    Vector4(0.0f, 0.0f, 0.0f, 1.0f)
);

Matrix4x4 Matrix4x4::Translate(const Vector3& pos)
{
    return Matrix4x4(
        Vector4(1.0f, 0.0f, 0.0f, pos.getX()),
        Vector4(0.0f, 1.0f, 0.0f, pos.getY()),
        Vector4(0.0f, 0.0f, 1.0f, pos.getZ()),
        Vector4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

Matrix4x4 Matrix4x4::Rotate(const Quaternion& rot)
{
    const float* q = rot;
    float x = q[0] * 2.0f;
    float y = q[1] * 2.0f;
    float z = q[2] * 2.0f;
    float w = q[3];

    float xx = q[0] * x;
    float xy = q[0] * y;
    float xz = q[0] * z;
    float yy = q[1] * y;
    float yz = q[1] * z;
    float zz = q[2] * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    // Ordering: ZYX
    // Get orthogonal matrix (3x3)
    return Matrix4x4(
        Vector4(
            1.0f - (yy + zz),
            xy - wz,
            xz + wy,
            0.0f
        ),
        Vector4(
            xy + wz,
            1.0f - (xx + zz),
            yz - wx,
            0.0f
        ),
        Vector4(
            xz - wy,
            yz + wx,
            1.0f - (xx + yy),
            0.0f
        ),
        Vector4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

Matrix4x4 Matrix4x4::Scale(const Vector3& scale)
{
    return Matrix4x4(
        Vector4(scale.getX(), 0.0f, 0.0f, 0.0f),
        Vector4(0.0f, scale.getY(), 0.0f, 0.0f),
        Vector4(0.0f, 0.0f, scale.getZ(), 0.0f),
        Vector4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

Matrix4x4 Matrix4x4::TRS(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
{
    const float* q = rot;
    float x = q[0] * 2.0f;
    float y = q[1] * 2.0f;
    float z = q[2] * 2.0f;
    float w = q[3];

    float xx = q[0] * x;
    float xy = q[0] * y;
    float xz = q[0] * z;
    float yy = q[1] * y;
    float yz = q[1] * z;
    float zz = q[2] * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    return Matrix4x4(
        Vector4(
            (1.0f - (yy + zz)) * scale.getX(),
            (xy - wz) * scale.getY(),
            (xz + wy) * scale.getZ(),
            pos.getX()
        ),
        Vector4(
            (xy + wz) * scale.getX(),
            (1.0f - (xx + zz)) * scale.getY(),
            (yz - wx) * scale.getZ(),
            pos.getY()
        ),
        Vector4(
            (xz - wy) * scale.getX(),
            (yz + wx) * scale.getY(),
            (1.0f - (xx + yy)) * scale.getZ(),
            pos.getZ()
        ),
        Vector4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

Matrix4x4 Matrix4x4::View(const Vector3& pos, const Quaternion& rot)
{
    Matrix4x4 rotMatrix = Matrix4x4::Rotate(rot);

    // Calculate "look at" vector (fwd) by using the camera transform rotation.
    Vector3 fwd = rotMatrix.multiplyVector(Vector3::forward);
    Vector3 up = rotMatrix.multiplyVector(Vector3::up);
    Vector3 right = Vector3::Cross(up, fwd);

    return Matrix4x4(
        Vector4(right.getX(), right.getY(), right.getZ(), -Vector3::Dot(right, pos)),
        Vector4(up.getX(), up.getY(), up.getZ(), -Vector3::Dot(up, pos)),
        Vector4(-fwd.getX(), -fwd.getY(), -fwd.getZ(), Vector3::Dot(fwd, pos)),
        Vector4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

Matrix4x4 Matrix4x4::Orthographic(float size, float aspect, float near, float far)
{
    return Matrix4x4(
        Vector4(1.0f / (size * aspect), 0.0f, 0.0f, 0.0f),
        Vector4(0.0f, 1.0f / size, 0.0f, 0.0f),
        Vector4(0.0f, 0.0f, -2.0f / (far - near), -(far + near) / (far - near)),
        Vector4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

Matrix4x4 Matrix4x4::Perspective(float fov, float aspect, float near, float far)
{
    float tanHalfFov = tan(degToRad(fov * 0.5f));

    return Matrix4x4(
        Vector4(1.0f / (tanHalfFov * aspect), 0.0f, 0.0f, 0.0f),
        Vector4(0.0f, 1.0f / tanHalfFov, 0.0f, 0.0f),
        Vector4(0.0f, 0.0f, -(far + near) / (far - near), -(2.0f * far * near) / (far - near)),
        Vector4(0.0f, 0.0f, -1.0f, 0.0f)
    );
}
