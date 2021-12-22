#include "GameContainer.h"

GameContainer* GameContainer::m_Instance = nullptr;

GameContainer::GameContainer(GLFWwindow* window) : m_MainWindow(window), m_FrameCountInLastSecond(0), m_LastFPSRecordTime(0.0)
{
    m_Instance = this;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);     // for skybox to work (renders at NDC Z 1.0)
    // Enable back face culling
    glEnable(GL_CULL_FACE);

    // Core
    Time::Init();
    Input::Init(window);
    ModelImporter::Init();
    MeshPrimitives::Init();
    Texture2D::CreateStaticTextures();

    int screenWidth, screenHeight;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);

    // Setup camera
    auto projection = Camera::PerspectiveProjection(
        /*nearClip=*/ 0.1f,
        /*farClip=*/ 5000.0f,
        /*fieldOfView=*/ 75.0f
    );
    m_MainCamera = new Camera(
        screenWidth,
        screenHeight,
        /*pos=*/ Vector3(0.0f, 1.0f, 0.0f),
        /*rot=*/ Vector3::zero,
        projection,
        CameraBufferFlags::Default | CameraBufferFlags::MotionVectors,
        /*deferred=*/ true
    );
    m_MainCamera->addDeferredEffect(new SSAO());
    m_MainCamera->addPostProcessEffect(new GlobalFog());
    m_MainCamera->addPostProcessEffect(new FXAA());
    SunShafts* shafts = new SunShafts();
    m_MainCamera->addPostProcessEffect(shafts);
    m_MainCamera->addPostProcessEffect(new Bloom());
    m_MainCamera->addPostProcessEffect(new MotionBlur());
    m_MainCamera->addPostProcessEffect(new Tonemapping());
    // m_MainCamera->addPostProcessEffect(new GaussianBlur());     // TODO: runtime toggle

    m_SmoothedCamMoveDir = Vector3::zero;

    // ImGui
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    m_ImGuiIO = &ImGui::GetIO();

    // Load scene.
    m_CurrentScene = new Scene();
    std::vector<Light*>& lights = m_CurrentScene->lights();

    if (lights.size() > 0)
        // We're assuming first light is a directional light / sun. Set that as the shaft's source.
        shafts->setSunTransform(lights[0]->getTransform());

    // GUI windows
    m_TexPickerWindow = new TexturePickerWindow(&HandleSelectedNewTexture);

    m_DebugTexturesWindow = new DebugTextureListWindow("Debug Buffers");
    m_DebugTexturesWindow->setOpen(false);
    DebugTextureListWindow& textureListWindow = *m_DebugTexturesWindow;
    m_MainCamera->addBuffersToDebugWindow(textureListWindow);
    for (Light* light : lights)
        light->addBuffersToDebugWindow(textureListWindow);

    m_DebugOverlayWindow = new DebugOverlayWindow(m_DebugTexturesWindow);
}

GameContainer::~GameContainer()
{
    if (m_Instance == this)
        m_Instance = nullptr;

    delete m_MainCamera;
    delete m_CurrentScene;
    delete m_DebugOverlayWindow;
    delete m_TexPickerWindow;
}

void GameContainer::update(double deltaTime)
{
    Time::Timestep(deltaTime);
    Input::Poll();

    handleMouseCursorState();

    if (m_CurrentScene)
        m_CurrentScene->update();

    handleCameraMovement(deltaTime);
    handleFPSCounter();

    // Toggle debug overlay with F1 key
    if (Input::GetKeyDown(KeyCode::F1))
    {
        m_DebugOverlayWindow->toggleVisible();
        m_LastFPSRecordTime = Time::GetTime();
        m_FrameCountInLastSecond = 0;
    }
}

void GameContainer::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (m_CurrentScene)
        m_CurrentScene->renderLightShadows();

    if (m_MainCamera)
    {
        m_MainCamera->draw(m_CurrentScene, /*drawSkybox=*/ true);
        m_MainCamera->blitToScreen();
    }

    onGUI();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GameContainer::handleCameraMovement(double deltaTime)
{
    if (!m_MainCamera)
        return;

    Transform* cameraTrans = m_MainCamera->getTransform();

    float dt = (float)deltaTime;
    bool allowInput = Input::GetMouseCursorState() == MouseCursorState::Locked;
    Vector3 targetMoveDir;

    if (allowInput)
    {
        // Mouse look
        Vector2 mouseDelta = Input::GetMouseMoveDelta() * 0.075f;
        Vector3 rotateDelta = rotationToRad(Vector3(mouseDelta.y, mouseDelta.x, 0.0f));
        cameraTrans->rotate(rotateDelta);

        // Free fly
        targetMoveDir = cameraTrans->transformDirection(getMoveAxis());
        bool space = Input::GetKey(KeyCode::Space);
        bool ctrl = Input::GetKey(KeyCode::LeftCtrl) || Input::GetKey(KeyCode::RightCtrl);

        if (space && !ctrl)
        {
            targetMoveDir.y += 1.0f;
        }
        else if (ctrl && !space)
        {
            targetMoveDir.y -= 1.0f;
        }

        if (targetMoveDir.getSqrMagnitude() > 1.0f)
            targetMoveDir.normalize();

        const float MOVE_SPEED = 6.0f;
        targetMoveDir *= MOVE_SPEED * dt;

        if (Input::GetKey(KeyCode::LeftShift))
            targetMoveDir *= 3.0f;
    }
    else
    {
        targetMoveDir = Vector3::zero;
    }

    // Approximate smoothing.
    const float MOVEMENT_SMOOTH_SPEED = 12.0f;
    m_SmoothedCamMoveDir = Vector3::Lerp(m_SmoothedCamMoveDir, targetMoveDir, dt * MOVEMENT_SMOOTH_SPEED);

    if (m_SmoothedCamMoveDir.getSqrMagnitude() > 0.000001f)
        cameraTrans->translate(m_SmoothedCamMoveDir);

    // Update and upload view projection.
    m_MainCamera->update();
}

void GameContainer::handleMouseCursorState()
{
    if (Input::GetMouseButtonDown(MouseButton::Left) || Input::GetMouseButtonDown(MouseButton::Right))
    {
        // We are not hovering over any GUI element
        if (!m_ImGuiIO->WantCaptureMouse)
            Input::SetMouseCursorState(MouseCursorState::Locked);
    }
    else if (Input::GetKeyDown(KeyCode::Escape))
    {
        Input::SetMouseCursorState(MouseCursorState::Default);
    }
}

void GameContainer::handleFPSCounter()
{
    if (!m_DebugOverlayWindow->getVisible())
        return;

    m_FrameCountInLastSecond++;
    double t = Time::GetTime();
    double elapsedTime = t - m_LastFPSRecordTime;

    if (elapsedTime >= FPS_UPDATE_INTERVAL)
    {
        m_DebugOverlayWindow->setFPS(m_FrameCountInLastSecond / static_cast<float>(elapsedTime));
        m_FrameCountInLastSecond = 0;
        m_LastFPSRecordTime = t;
    }
}

void GameContainer::onGUI()
{
    m_DebugOverlayWindow->draw();
    m_TexPickerWindow->draw();
    m_DebugTexturesWindow->draw();
}

void GameContainer::HandleSelectedNewTexture(const std::string& path)
{
    if (!m_Instance)
        return;

    m_Instance->m_CurrentScene->setNewTexture(path);
}

Vector3 GameContainer::getMoveAxis() const
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
