#pragma once

#include "../../Core/Scene.h"
#include "../../GUI/DebugTextureListWindow.h"
#include "../Material.h"
#include "../Transform.h"

using std::cout;
using std::endl;


// Forward declaration
class Camera;
class Scene;


class Light
{
public:
    virtual void setUniforms(Material& mat) const = 0;
    virtual void update() = 0;
    virtual void renderShadows(const Scene* scene) const = 0;
    virtual Matrix4x4 getLightMatrix() const = 0;
    virtual void addBuffersToDebugWindow(DebugTextureListWindow& window) const = 0;
protected:
    Light(const Vector3& pos, const Vector3& rot);
    Transform* m_Transform;
    Material* m_ShadowMat;
};

class DirectionalLight : public Light
{
    void setUniforms(Material& mat) const override;
    void update() override;
    void renderShadows(const Scene* scene) const override;
    Matrix4x4 getLightMatrix() const override;
    void addBuffersToDebugWindow(DebugTextureListWindow& window) const override;
public:
    DirectionalLight(const Vector3& pos, const Vector3& rot);
    ~DirectionalLight();
private:
    Camera* m_DepthCamera;
};
