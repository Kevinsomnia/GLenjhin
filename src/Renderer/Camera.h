#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "../Core/Scene.h"
#include "../GUI/GUIWindows.h"
#include "../Math/Matrix4x4.h"
#include "Texture/Texture.h"
#include "PostProcessing/PostProcessing.h"
#include "Transform.h"

using std::cout;
using std::endl;


// Forward declaration
class ImageEffect;
class ImageEffectChain;
class Scene;


class Camera
{
public:
    Camera(const Vector3& pos, const Vector3& rot, float fieldOfView, float nearClip, float farClip);
    ~Camera();
    void update();
    void draw(const Scene* scene);
    void addImageEffect(ImageEffect* effect);
    void addBuffersToDebugWindow(DebugTextureListWindow& window) const;
    Transform* getTransform() const;
    Matrix4x4 getViewProjMatrix() const;

    // X = near clip
    // Y = far clip
    // Z = far - near
    // W = 2.0 * near * far
    Vector4 getProjectionParams() const;
private:
    Transform* m_Transform;
    BufferTexture* m_BufferTex;
    ImageEffectChain* m_ImageEffectChain;
    Matrix4x4 m_ViewProjMatrix;
    Vector4 m_ProjectionParams;
    float m_FieldOfView;
    float m_NearClip;
    float m_FarClip;
};

#endif // CAMERA_H
