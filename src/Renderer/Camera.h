#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "../Core/Scene.h"
#include "../GUI/GUIWindows.h"
#include "../Math/Matrix4x4.h"
#include "Deferred/GeometryBuffers.h"
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
    Camera(const Vector3& pos, const Vector3& rot, float fieldOfView, float nearClip, float farClip, bool deferred);
    ~Camera();
    void update();
    void draw(Scene* scene);
    void blitToScreen() const;
    void addImageEffect(ImageEffect* effect);
    void addBuffersToDebugWindow(DebugTextureListWindow& window) const;
    inline bool isDeferred() const { return m_GBuffers; }
    Texture2D* getDepthTexture() { return isDeferred() ? m_GBuffers->depthTexture() : m_RenderTargetBuffer->depthTexture(); }
    GeometryBuffers* getGBuffers() const { return m_GBuffers; }
    Transform* getTransform() const { return m_Transform; }
    Matrix4x4 getViewProjMatrix() const { return m_ViewProjMatrix; }
    // X = near clip
    // Y = far clip
    // Z = far - near
    // W = 2.0 * near * far
    Vector4 getProjectionParams() const { return m_ProjectionParams; }
private:
    Transform* m_Transform;
    GeometryBuffers* m_GBuffers;
    BufferTexture* m_RenderTargetBuffer;
    ImageEffectChain* m_ImageEffectChain;
    Material* m_DeferredGeometryMat;
    Material* m_DeferredLightingMat;
    Material* m_BlitMat;
    FullscreenTriangle* m_FullscreenTriangle;
    Matrix4x4 m_ViewProjMatrix;
    Vector4 m_ProjectionParams;
    float m_FieldOfView;
    float m_NearClip;
    float m_FarClip;
};

#endif // CAMERA_H
