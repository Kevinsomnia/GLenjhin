#include "Scene.h"

Scene::Scene()
{
    // Setup lighting
    Light* sun = new DirectionalLight(Vector3::zero, rotationToRad(Vector3(28.5f, 36.0f, 0.0f)));
    m_Lights.push_back(sun);

    m_Skybox = new Skybox("res\\textures\\DaySkybox.png");

    // Load shader and material
    m_CurrMat = new Material(new Shader("res\\shaders\\StandardSurface.glsl"));
    m_CurrTexture = new Texture2D("res\\textures\\test_grid.png");
    m_CurrMat->setTexture("u_MainTex", m_CurrTexture);

    Material* sphereMat = new Material(new Shader("res\\shaders\\StandardSurface.glsl"));   // yes this will leak memory. temp solution.
    sphereMat->setColor("u_EmissionColor", Color(0.5f, 1.5f, 0.25f));

    Entity* plane = new Entity(Vector3::zero, rotationToRad(Vector3(-90.0f, 180.0f, 0.0f)), Vector3::one * 20.0f);
    plane->setupRenderer(MeshPrimitives::quad, m_CurrMat);
    m_Entities.push_back(plane);

    Entity* wall = new Entity(Vector3(0.5f, 1.5f, 0.0f), Vector3::zero, Vector3(0.25f, 3.0f, 15.0f));
    wall->setupRenderer(MeshPrimitives::cube, m_CurrMat);
    m_Entities.push_back(wall);

    for (int i = 0; i < 10; i++)
    {
        Entity* entity = new Entity(Vector3(-0.65f, 0.45f, 2.95f), Vector3::zero, Vector3::one * 0.4f);
        bool isSphere = i % 5 == 0;
        entity->setupRenderer(isSphere ? MeshPrimitives::sphere : MeshPrimitives::cube, isSphere ? sphereMat : m_CurrMat);
        m_Entities.push_back(entity);
        m_DynamicEntities.push_back(entity);
    }
}

Scene::~Scene()
{
    delete m_CurrMat;
    delete m_CurrTexture;

    for (Entity* entity : m_Entities)
        delete entity;

    for (Light* light : m_Lights)
        delete light;

    m_Entities.clear();
    m_DynamicEntities.clear();
    m_Lights.clear();
}

void Scene::update()
{
    double t = Time::GetTime();

    for (Light* light : m_Lights)
        light->update();

    for (size_t i = 0; i < m_DynamicEntities.size(); i++)
    {
        t += 2.48529;
        Entity* entity = m_DynamicEntities[i];

        entity->getTransform()->setPosition(Vector3(
            (float)cos(t * 0.7),
            (float)sin(t * 0.8) * 0.5f + 2.0f,
            (float)sin(t * 0.175) * 7.0f
        ));
        entity->getTransform()->setRotation(rotationToRad(Vector3(
            (float)t * 25.0f,
            (float)t * 30.0f,
            (float)t * 35.0f
        )));
    }
}

void Scene::drawGeometryPass(const Camera& camera, Material& geometryMat) const
{
    geometryMat.setMatrix("u_VP", camera.getViewProjMatrix());
    geometryMat.bind();

    for (size_t i = 0; i < m_Entities.size(); i++)
        m_Entities[i]->drawGeometryPass(geometryMat);
}

void Scene::drawShadowPass(const Light& light, Material& shadowMat) const
{
    shadowMat.setMatrix("u_L", light.getLightMatrix());
    shadowMat.bind();

    for (size_t i = 0; i < m_Entities.size(); i++)
        m_Entities[i]->drawShadowPass(shadowMat);
}

void Scene::drawSkybox(const Camera& camera) const
{
    if (m_Skybox)
        m_Skybox->draw(camera.getViewProjMatrix());
}

void Scene::drawEntities(const Camera& camera) const
{
    // Only pass lights to each entity if the camera is rendering forward.
    const std::vector<Light*>& lights = camera.isDeferred() ? m_EmptyLightsList : m_Lights;

    for (size_t i = 0; i < m_Entities.size(); i++)
        m_Entities[i]->draw(camera, lights);
}

void Scene::renderLightShadows() const
{
    for (Light* light : m_Lights)
        light->renderShadows(this);
}

void Scene::setNewTexture(const std::string& texturePath)
{
    if (m_CurrTexture)
        delete m_CurrTexture;

    m_CurrTexture = new Texture2D(texturePath);
    m_CurrMat->setTexture("u_MainTex", m_CurrTexture);
}
