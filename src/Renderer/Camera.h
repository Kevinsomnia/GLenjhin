#pragma once

#include <assert.h>

#include "../Core/Scene.h"
#include "../GUI/DebugWindow.h"
#include "../Math/Matrix4x4.h"
#include "Deferred/GeometryBuffers.h"
#include "ImageEffects/Deferred/DeferredEffectChain.h"
#include "ImageEffects/PostProcess/PostProcessEffectChain.h"

using std::cout;
using std::endl;


// Forward declaration
class DeferredEffect;
class DeferredEffectChain;
class PostProcessEffect;
class PostProcessEffectChain;
class Scene;
class Transform;


// Controls which buffers the camera should render to.
enum class CameraBufferFlags : uint32_t
{
    None = 0,
    Color = 1 << 0,
    Depth = 1 << 1,
    MotionVectors = 1 << 2,
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
    void draw(Scene* scene, bool drawSkybox);
    void blitToScreen() const;
    void addDeferredEffect(DeferredEffect* effect);
    void addPostProcessEffect(PostProcessEffect* effect);
    void addBuffersToDebugWindow(DebugWindow& window) const;
    Vector3 worldToViewportPoint(const Vector3& pos) const;
    inline bool isDeferred() const { return m_GBuffers; }
    BufferTexture* renderTargetBuffer() const { return m_RenderTargetBuffer; }
    Texture2D* colorTexture() const { return m_RenderTargetBuffer->colorTexture(); }
    Texture2D* depthTexture() const { return m_GBuffers ? m_GBuffers->depthTexture() : m_RenderTargetBuffer->depthTexture(); }
    Texture2D* motionVectorsTexture() const { return m_GBuffers ? m_GBuffers->motionVectorsTexture() : nullptr; }
    GeometryBuffers* gBuffers() const { return m_GBuffers; }
    Transform* transform() const { return m_Transform; }
    Matrix4x4 prevViewProjectionMatrix() const { return m_PrevViewProjectionMatrix; }
    Matrix4x4 viewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    Matrix4x4 viewMatrix() const { return m_ViewMatrix; }
    Matrix4x4 projectionMatrix() const { return m_ProjectionMatrix; }
    // X = near clip
    // Y = far clip
    // Z = far - near
    // W = 2.0 * near * far
    Vector4 projectionParams() const { return m_ProjectionParams; }
    CameraBufferFlags bufferFlags() const { return m_BufferFlags; }
private:
    Transform* m_Transform;
    GeometryBuffers* m_GBuffers;
    BufferTexture* m_RenderTargetBuffer;
    PostProcessEffectChain* m_PostProcessChain;
    DeferredEffectChain* m_DeferredChain;
    Material* m_DeferredGeometryMat;
    Material* m_DeferredLightingMat;
    Material* m_BgMotionVectorsMat;
    Material* m_BlitMat;
    Matrix4x4 m_PrevViewProjectionMatrix;
    Matrix4x4 m_ViewProjectionMatrix;

    Vector4 m_ProjectionParams;
    Matrix4x4 m_ViewMatrix;
    Matrix4x4 m_ProjectionMatrix;
    float m_NearClip;
    float m_FarClip;
    float m_FieldOfView;
    float m_OrthoSize;
    CameraBufferFlags m_BufferFlags;

    void renderBackgroundMotionVectors();
};
