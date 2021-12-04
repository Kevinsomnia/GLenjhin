#ifndef LIGHTING_H
#define LIGHTING_H

#include <iostream>

#include "../../Core/Scene.h"
#include "../../GUI/GUIWindows.h"
#include "../../Math/Math.h"
#include "../Camera.h"
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
    virtual void bind(Material& mat) const = 0;
    virtual void update() = 0;
    virtual void renderShadows(const Scene* scene) const = 0;
    virtual Matrix4x4 getLightMatrix() const = 0;
    virtual Texture2D* getShadowMap() const = 0;
protected:
    Light(const Vector3& pos, const Vector3& rot);
    Transform* m_Transform;
    Material* m_ShadowMat;
};

class DirectionalLight : public Light
{
    void bind(Material& mat) const override;
    void update() override;
    void renderShadows(const Scene* scene) const override;
    Matrix4x4 getLightMatrix() const override;
    Texture2D* getShadowMap() const override;
public:
    DirectionalLight(const Vector3& pos, const Vector3& rot);
    ~DirectionalLight();
private:
    Camera* m_DepthCamera;
};

#endif
