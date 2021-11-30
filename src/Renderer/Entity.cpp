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

void Entity::drawGeometryPass(const Camera& cam, Material& geometryMat) const
{
    if (m_Renderer)
    {
        geometryMat.setMatrix("u_Model", m_Transform->getTRS());
        geometryMat.setTexture("u_AlbedoTex", m_Renderer->getMaterial()->getTexture("u_MainTex"));
        geometryMat.updateUniforms();

        m_Renderer->drawMeshDirect();
    }
}

void Entity::draw(const Camera& cam, const std::vector<Light*>& lights) const
{
    if (m_Renderer)
    {
        m_Renderer->draw(
            cam.getTransform()->getPosition(),
            cam.getViewProjMatrix(),
            m_Transform->getTRS(),
            lights
        );
    }
}
