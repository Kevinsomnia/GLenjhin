#include "Entity.h"

Entity::Entity(const Vector3& position, const Vector3& rotation, const Vector3& scale)
    : m_Renderer(nullptr)
{
    m_Transform = new Transform(position, rotation, scale);
}

Entity::~Entity()
{
    delete m_Transform;

    if (m_Renderer)
    {
        delete m_Renderer;
        m_Renderer = nullptr;
    }
}

void Entity::setupRenderer(Mesh* mesh, Material* material)
{
    if (m_Renderer)
    {
        cerr << "Renderer is already set up!" << endl;
        return;
    }

    m_Renderer = new MeshRenderer(mesh, material);
}

void Entity::drawGeometryPass(Material& geometryMat) const
{
    if (m_Renderer)
    {
        Material* entityMat = m_Renderer->material();

        geometryMat.setMatrix("u_Model", m_Transform->getTRS());
        geometryMat.setTexture("u_AlbedoTex", entityMat->getTexture("u_MainTex"));
        geometryMat.setColor("u_EmissionColor", entityMat->getColor("u_EmissionColor"));
        geometryMat.updateUniforms();

        m_Renderer->drawMeshDirect();
    }
}

void Entity::drawShadowPass(Material& shadowMat) const
{
    if (m_Renderer)
    {
        shadowMat.setMatrix("u_Model", m_Transform->getTRS());
        shadowMat.updateUniforms();

        m_Renderer->drawMeshDirect();
    }
}

void Entity::draw(const Camera& cam, const std::vector<Light*>& lights) const
{
    if (m_Renderer)
    {
        m_Renderer->draw(
            /*cameraPos=*/ cam.getTransform()->getPosition(),
            /*vp=*/ cam.getViewProjMatrix(),
            /*model=*/ m_Transform->getTRS(),
            lights
        );
    }
}
