#ifndef CAMERA_H
#define CAMERA_H

#include <assert.h>
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


// Controls which buffers the camera should render to.
enum class CameraBufferFlags : uint32_t
{
    None = 0,
    Color = 1 << 0,
    Depth = 1 << 1,
    Default = Color | Depth
};
constexpr CameraBufferFlags operator |(const CameraBufferFlags lhs, const CameraBufferFlags rhs) { return CameraBufferFlags(uint32_t(lhs) | uint32_t(rhs)); }
constexpr CameraBufferFlags operator |=(const CameraBufferFlags lhs, const CameraBufferFlags rhs) { return CameraBufferFlags(uint32_t(lhs) | uint32_t(rhs)); }
constexpr CameraBufferFlags operator &(const CameraBufferFlags lhs, const CameraBufferFlags rhs) { return CameraBufferFlags(uint32_t(lhs) & uint32_t(rhs)); }
constexpr CameraBufferFlags operator &=(const CameraBufferFlags lhs, const CameraBufferFlags rhs) { return CameraBufferFlags(uint32_t(lhs) & uint32_t(rhs)); }
constexpr CameraBufferFlags operator ~(const CameraBufferFlags flags) { return CameraBufferFlags(~uint32_t(flags)); }
inline std::ostream& operator<<(std::ostream& os, const CameraBufferFlags& flags)
{
    os << static_cast<uint32_t>(flags);
    return os;
}


class Camera
{
public:
    struct Projection
    {
    public:
        float nearClip;
        float farClip;
    protected:
        Projection() = delete;
        Projection(float nearClip, float farClip);
        virtual ~Projection();
    };

    struct PerspectiveProjection : public Projection
    {
        PerspectiveProjection(float nearClip, float farClip, float fieldOfView);

        float fieldOfView;
    };

    struct OrthographicProjection : public Projection
    {
        OrthographicProjection(float nearClip, float farClip, float size);

        float size;
    };

    Camera(uint32_t pixelWidth, uint32_t pixelHeight, const Vector3& pos, const Vector3& rot, Projection& projection, CameraBufferFlags bufferFlags, bool deferred);
    ~Camera();
    void update();
    void draw(Scene* scene);
    void blitToScreen() const;
    void addImageEffect(ImageEffect* effect);
    void addBuffersToDebugWindow(DebugTextureListWindow& window) const;
    inline bool isDeferred() const { return m_GBuffers; }
    BufferTexture* getRenderTargetBuffer() const { return m_RenderTargetBuffer; }
    Texture2D* getColorTexture() const { return m_RenderTargetBuffer->colorTexture(); }
    Texture2D* getDepthTexture() const { return isDeferred() ? m_GBuffers->depthTexture() : m_RenderTargetBuffer->depthTexture(); }
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
    Matrix4x4 m_ProjMatrix;
    float m_NearClip;
    float m_FarClip;
    float m_FieldOfView;
    float m_OrthoSize;
};

#endif // CAMERA_H
