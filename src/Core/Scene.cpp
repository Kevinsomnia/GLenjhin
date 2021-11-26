#include "Scene.h"

Scene::Scene()
{
    // Setup camera
    m_Camera = new Camera(
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3::zero,
        70.0f,		// FOV
        0.1f,		// Near
        1000.0f		// Far
    );

    // Setup lighting
    Light* sun = new DirectionalLight(Vector3::zero, rotationToRad(Vector3(30.0f, 50.0f, 0.0f)));
    m_Lights.push_back(sun);

    // Load shader and material
    Shader* surfaceShader = new Shader("res\\shaders\\StandardSurface.shader");
    m_CurrMat = new Material(surfaceShader);
    m_CurrTexture = new Texture2D("res\\textures\\test_grid.png");
    m_CurrMat->setTexture("u_MainTex", m_CurrTexture);

    Entity* plane = new Entity(Vector3::zero, rotationToRad(Vector3(-90.0f, 180.0f, 0.0f)), Vector3::one * 20.0f);
    plane->setupRenderer(MeshPrimitives::quad, m_CurrMat);
    m_Entities.push_back(plane);

    for (int i = 0; i < 10; i++)
    {
        Entity* entity = new Entity(Vector3(-0.65f, 0.45f, 2.95f), Vector3::zero, Vector3::one * 0.4f);
        entity->setupRenderer(i % 2 == 0 ? MeshPrimitives::sphere : MeshPrimitives::cube, m_CurrMat);
        m_Entities.push_back(entity);
    }
}

Scene::~Scene()
{
    delete m_Camera;
    delete m_CurrMat;
    delete m_CurrTexture;

    for (Entity* entity : m_Entities)
        delete entity;

    for (Light* light : m_Lights)
        delete light;

    m_Entities.clear();
    m_Lights.clear();
}

void Scene::update()
{
    double t = Time::GetTime();
    double dt = Time::GetDeltaTime();

    for (size_t i = 1; i < m_Entities.size(); i++)
    {
        t += 2.48529;
        Entity* entity = m_Entities[i];

        entity->getTransform()->setPosition(Vector3(
            (float)cos(t * 0.5),
            (float)sin(t * 0.6) * 0.5f + 3.0f,
            (float)sin(t * 0.1) * 7.0f
        ));
        entity->getTransform()->setRotation(rotationToRad(Vector3(
            (float)t * 25.0f,
            (float)t * 30.0f,
            (float)t * 35.0f
        )));
    }

    if (m_Camera)
    {
        bool allowInput = Input::GetMouseCursorState() == MouseCursorState::Locked;

        if (allowInput)
        {
            // Mouse look
            Vector2 mouseDelta = Input::GetMouseMoveDelta() * 0.075f;
            Vector3 rotateDelta = rotationToRad(Vector3(mouseDelta.getY(), mouseDelta.getX(), 0.0f));
            m_Camera->getTransform()->rotate(rotateDelta);

            // Free fly
            float moveSpeed = 6.0f;

            if (Input::GetKey(KeyCode::LeftShift))
            {
                moveSpeed *= 3.0f;
            }

            Vector3 moveDelta = getMoveAxis() * moveSpeed * (float)dt;
            m_Camera->getTransform()->translate(moveDelta, Space::Local);

            bool space = Input::GetKey(KeyCode::Space);
            bool ctrl = Input::GetKey(KeyCode::LeftCtrl) || Input::GetKey(KeyCode::RightCtrl);
            float y = 0.0f;

            if (space && !ctrl)
            {
                y = 1.0f;
            }
            else if (ctrl && !space)
            {
                y = -1.0f;
            }

            m_Camera->getTransform()->translate(Vector3(0.0f, y, 0.0f) * moveSpeed * (float)dt, Space::World);
        }

        // Update and upload view projection.
        m_Camera->update();
    }
}

void Scene::draw()
{
    if (!m_Camera)
        return;

    for (size_t i = 0; i < m_Entities.size(); i++)
        m_Entities[i]->draw(*m_Camera, m_Lights);
}

void Scene::setNewTexture(const std::string& texturePath)
{
    if (m_CurrTexture)
        delete m_CurrTexture;

    m_CurrTexture = new Texture2D(texturePath);
    m_CurrMat->setTexture("u_MainTex", m_CurrTexture);
}

Vector3 Scene::getMoveAxis() const
{
    bool w = Input::GetKey(KeyCode::W);
    bool s = Input::GetKey(KeyCode::S);
    bool a = Input::GetKey(KeyCode::A);
    bool d = Input::GetKey(KeyCode::D);

    float x = 0.0f, z = 0.0f;

    if (d && !a)
    {
        x = 1.0f;
    }
    else if (a && !d)
    {
        x = -1.0f;
    }

    if (w && !s)
    {
        z = 1.0f;
    }
    else if (s && !w)
    {
        z = -1.0f;
    }

    Vector3 result(x, 0.0f, z);

    if (result.getSqrMagnitude() > 1.0f)
        result.normalize();

    return result;
}
