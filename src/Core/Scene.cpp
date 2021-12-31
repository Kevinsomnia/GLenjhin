#include "Scene.h"

Scene::Scene()
{
    // Setup lighting
    Light* sun = new DirectionalLight(Vector3::zero, rotationToRad(Vector3(28.5f, 36.0f, 0.0f)));
    m_Lights.push_back(sun);

    m_Skybox = new Skybox("res\\textures\\DaySkybox.png");

    // Load models
    m_DragonModel = ModelImporter::LoadFBX("res\\models\\dragon\\dragon.fbx");
    m_CustomModel = ModelImporter::LoadFBX("res\\models\\m4\\M4.fbx");

    // Load textures
    m_GroundAlbedo = new Texture2D("res\\textures\\cavern-deposits\\cavern-deposits_albedo.png", /*generateMipmaps=*/ true, /*readable=*/ false, /*sRGB=*/ true);
    m_GroundAlbedo->setAnisotropicFilterLevel(4);
    Texture2D* groundNormalMap = new Texture2D("res\\textures\\cavern-deposits\\cavern-deposits_normal.png", /*generateMipmaps=*/ true, /*readable=*/ false, /*sRGB=*/ false);
    groundNormalMap->setFilterMode(TextureFilterMode::Trilinear);
    groundNormalMap->setAnisotropicFilterLevel(4);
    Texture2D* groundMSA = new Texture2D("res\\textures\\cavern-deposits\\cavern-deposits_MSA.png", /*generateMipmaps=*/ true, /*readable=*/ false, /*sRGB=*/ true);
    groundMSA->setAnisotropicFilterLevel(4);
    Texture2D* groundHeight = new Texture2D("res\\textures\\cavern-deposits\\cavern-deposits_height.png", /*generateMipmaps=*/ true, /*readable=*/ false, /*sRGB=*/ false);
    groundHeight->setAnisotropicFilterLevel(4);

    Texture2D* metalAlbedo = new Texture2D("res\\textures\\streaky-metal\\streaky-metal_albedo.png", /*generateMipmaps=*/ true, /*readable=*/ false, /*sRGB=*/ true);
    metalAlbedo->setAnisotropicFilterLevel(4);
    Texture2D* metalNormalMap = new Texture2D("res\\textures\\streaky-metal\\streaky-metal_normal.png", /*generateMipmaps=*/ true, /*readable=*/ false, /*sRGB=*/ false);
    metalNormalMap->setFilterMode(TextureFilterMode::Trilinear);
    metalNormalMap->setAnisotropicFilterLevel(4);
    Texture2D* metalMSA = new Texture2D("res\\textures\\streaky-metal\\streaky-metal_MSA.png", /*generateMipmaps=*/ true, /*readable=*/ false, /*sRGB=*/ true);
    metalMSA->setAnisotropicFilterLevel(4);

    Texture2D* testGridAlbedo = new Texture2D("res\\textures\\test_grid.png", /*generateMipmaps=*/ true, /*readable=*/ false, /*sRGB=*/ true);
    testGridAlbedo->setAnisotropicFilterLevel(4);

    // Load shader and material
    m_GroundMat = new Material(Shader::Load("res\\shaders\\StandardSurface.glsl"));
    m_GroundMat->setTexture("u_AlbedoTex", m_GroundAlbedo);
    m_GroundMat->setTexture("u_NormalTex", groundNormalMap);
    m_GroundMat->setTexture("u_MSATex", groundMSA);
    m_GroundMat->setTexture("u_HeightTex", groundHeight);
    m_GroundMat->setFloat("u_HeightScale", 0.007f);
    m_GroundMat->setVector2("u_TileSize", Vector2(5.0f, 5.0f));

    Material* wallMat = new Material(Shader::Load("res\\shaders\\StandardSurface.glsl"));
    wallMat->setTexture("u_AlbedoTex", metalAlbedo);
    wallMat->setTexture("u_NormalTex", metalNormalMap);
    wallMat->setTexture("u_MSATex", metalMSA);
    wallMat->setVector2("u_TileSize", Vector2(3.75f, 0.75f));

    Material* basicMat = new Material(Shader::Load("res\\shaders\\StandardSurface.glsl"));
    basicMat->setTexture("u_AlbedoTex", testGridAlbedo);
    basicMat->setTexture("u_NormalTex", metalNormalMap);
    basicMat->setTexture("u_MSATex", groundMSA);
    basicMat->setVector2("u_TileSize", Vector2::one);

    Material* emissiveMat = new Material(Shader::Load("res\\shaders\\StandardSurface.glsl"));   // yes this will leak memory. temp solution.
    emissiveMat->setTexture("u_AlbedoTex", Texture2D::blackTexture);
    emissiveMat->setTexture("u_MSATex", Texture2D::blackTexture);
    emissiveMat->setColor("u_EmissionColor", Color(0.5f, 1.5f, 0.25f));

    Material* whiteMat = new Material(Shader::Load("res\\shaders\\StandardSurface.glsl"));      // yes this will leak memory. temp solution.
    whiteMat->setTexture("u_AlbedoTex", Texture2D::whiteTexture);
    whiteMat->setTexture("u_NormalTex", metalNormalMap);
    whiteMat->setTexture("u_MSATex", Texture2D::msaDefaultTexture);

    // Create entities
    Entity* plane = new Entity(Vector3::zero, rotationToRad(Vector3(-90.0f, 180.0f, 0.0f)), Vector3::one * 20.0f);
    plane->setupRenderer(MeshPrimitives::quad, m_GroundMat);
    m_Entities.push_back(plane);

    m_WallEntity = new Entity(Vector3(0.5f, 1.5f, 0.0f), Vector3::zero, Vector3(0.25f, 3.0f, 15.0f));
    m_WallEntity->setupRenderer(MeshPrimitives::cube, wallMat);
    m_Entities.push_back(m_WallEntity);

    Entity* dragon = new Entity(Vector3(-4.5f, 1.0f, 1.0f), rotationToRad(Vector3(-90.0f, 30.0f, 0.0f)), Vector3::one * 0.1f);
    dragon->setupRenderer(m_DragonModel.meshes[0], whiteMat);
    m_Entities.push_back(dragon);

    for (size_t i = 0; i < m_CustomModel.meshes.size(); i++)
    {
        Entity* model = new Entity(Vector3(-2.0f, 0.5f, -1.0f), rotationToRad(Vector3(45.0f, 120.0f, 15.0f)), Vector3::one);
        model->setupRenderer(m_CustomModel.meshes[i], whiteMat);
        m_Entities.push_back(model);
    }

    std::array<Mesh*, 5> primitives { MeshPrimitives::cube, MeshPrimitives::capsule, MeshPrimitives::cylinder, MeshPrimitives::cube, MeshPrimitives::sphere };

    for (int i = 0; i < 10; i++)
    {
        Entity* entity = new Entity(Vector3::zero, Vector3::zero, Vector3::one * 0.4f);
        Mesh* primitive = primitives[i % primitives.size()];
        entity->setupRenderer(primitive, primitive == MeshPrimitives::sphere ? emissiveMat : basicMat);
        entity->transform()->setParent(m_WallEntity->transform());
        m_Entities.push_back(entity);
        m_DynamicEntities.push_back(entity);
    }

    for (int i = 0; i < 3; i++)
    {
        Entity* entity = new Entity(Vector3::zero, Vector3::zero, Vector3::one * 0.5f);
        Mesh* primitive = primitives[i % primitives.size()];
        entity->setupRenderer(primitive, basicMat);
        m_Entities.push_back(entity);
        m_FastEntities.push_back(entity);
    }
}

Scene::~Scene()
{
    delete m_GroundMat;
    delete m_GroundAlbedo;
    if (m_Skybox)
        delete m_Skybox;

    for (Entity* entity : m_Entities)
        delete entity;

    for (Light* light : m_Lights)
        delete light;

    m_DragonModel.cleanup();
    m_CustomModel.cleanup();

    m_Entities.clear();
    m_DynamicEntities.clear();
    m_FastEntities.clear();
    m_Lights.clear();
}

void Scene::update()
{
    // Run entity logic as early in the frame as possible.
    for (Entity* entity : m_Entities)
        entity->earlyUpdate();

    for (Light* light : m_Lights)
        light->update();

    userUpdate();
}

void Scene::drawGeometryPass(const Camera& camera, Material& geometryMat) const
{
    geometryMat.setMatrix4x4("u_PrevVP", camera.prevViewProjectionMatrix());
    geometryMat.setMatrix4x4("u_CurrVP", camera.viewProjectionMatrix());
    geometryMat.setVector3("u_CameraPos", camera.transform()->localPosition());

    for (size_t i = 0; i < m_Entities.size(); i++)
        m_Entities[i]->drawGeometryPass(geometryMat);
}

void Scene::drawShadowPass(const Light& light, Material& shadowMat) const
{
    shadowMat.setMatrix4x4("u_L", light.lightMatrix());

    for (size_t i = 0; i < m_Entities.size(); i++)
        m_Entities[i]->drawShadowPass(shadowMat);
}

void Scene::drawSkybox(const Camera& camera) const
{
    if (m_Skybox)
        m_Skybox->draw(camera.viewProjectionMatrix());
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
    if (m_GroundAlbedo)
        delete m_GroundAlbedo;

    m_GroundAlbedo = new Texture2D(texturePath, /*generateMipmap=*/ true, /*readable=*/ false, /*sRGB=*/ true);
    m_GroundMat->setTexture("u_AlbedoTex", m_GroundAlbedo);
}

void Scene::userUpdate()
{
    float t = (float)Time::GetTime();

    m_WallEntity->transform()->setLocalRotation(rotationToRad(Vector3(0.0f, t * 10.0f, 0.0f)));

    for (size_t i = 0; i < m_DynamicEntities.size(); i++)
    {
        float ti = t + (2.5f * i);
        Transform* trans = m_DynamicEntities[i]->transform();

        trans->setLocalPosition(Vector3(
            cos(ti * 0.7f),
            sin(ti * 0.8f) * 0.5f + 2.0f,
            sin(ti * 0.175f) * 7.0f
        ));
        trans->setLocalRotation(rotationToRad(Vector3(
            ti * 25.0f,
            ti * 30.0f,
            ti * 35.0f
        )));
    }

    for (size_t i = 0; i < m_FastEntities.size(); i++)
    {
        float ti = (t * 9.0f) + (6.0f * i);
        Transform* trans = m_FastEntities[i]->transform();

        trans->setLocalPosition(Vector3(
            10.0f,
            sin(ti) + 2.0f,
            sin(ti * 0.25f) * 10.0f
        ));
        trans->setLocalRotation(rotationToRad(Vector3(
            ti * 155.0f,
            ti * 175.0f,
            ti * 205.0f
        )));
    }
}
