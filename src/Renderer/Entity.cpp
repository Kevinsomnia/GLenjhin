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

        geometryMat.setMatrix4x4("u_Model", m_Transform->getTRS());
        geometryMat.setColor("u_EmissionColor", entityMat->getColor("u_EmissionColor"));

        Texture* albedo = entityMat->getTexture("u_AlbedoTex");
        if (!albedo)
            albedo = Texture2D::whiteTexture;

        Texture* nrm = entityMat->getTexture("u_NormalTex");
        if (!nrm)
            nrm = Texture2D::normalMapDefaultTexture;

        // MSA = Metallic/Smoothness/AO
        Texture* msa = entityMat->getTexture("u_MSATex");
        if (!msa)
            msa = Texture2D::msaDefaultTexture;

        Texture* height = entityMat->getTexture("u_HeightTex");
        if (!height)
            height = Texture2D::whiteTexture;

        Vector2 tileSize = entityMat->getVector2("u_TileSize");
        if (tileSize.getSqrMagnitude() == 0.0f)
            tileSize = Vector2::one;

        geometryMat.setTexture("u_AlbedoTex", albedo);
        geometryMat.setTexture("u_NormalTex", nrm);
        geometryMat.setTexture("u_MSATex", msa);
        geometryMat.setTexture("u_HeightTex", height);
        geometryMat.setVector2("u_HeightTexelSize", height->texelSize());
        geometryMat.setFloat("u_HeightScale", entityMat->getFloat("u_HeightScale"));
        geometryMat.setVector2("u_TileSize", tileSize);

        geometryMat.bind();

        m_Renderer->drawMeshDirect();
    }
}

void Entity::drawShadowPass(Material& shadowMat) const
{
    if (m_Renderer)
    {
        shadowMat.setMatrix4x4("u_Model", m_Transform->getTRS());
        shadowMat.bind();

        m_Renderer->drawMeshDirect();
    }
}

void Entity::draw(const Camera& cam, const std::vector<Light*>& lights) const
{
    if (m_Renderer)
    {
        m_Renderer->draw(
            cam.getTransform()->getPosition(),
            cam.getViewProjectionMatrix(),
            /*model=*/ m_Transform->getTRS(),
            lights
        );
    }
}
