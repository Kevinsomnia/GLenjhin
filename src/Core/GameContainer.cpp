#include "GameContainer.h"

GameContainer* GameContainer::m_Instance = nullptr;

GameContainer::GameContainer(GLFWwindow* window) : m_MainWindow(window), m_FrameCountInLastSecond(0), m_LastFPSRecordTime(0.0)
{
    m_Instance = this;

    glEnable(GL_DEPTH_TEST);
    // Enable back face culling
    glEnable(GL_CULL_FACE);

    // Core
    Time::Init();
    Input::Init(window);
    MeshPrimitives::Init();

    // HDR screen buffer
    m_ScreenBuffer = new BufferTexture(1600, 900, /*depth=*/ 32, TextureFormat::RGBAHalf);

    // Image effects
    m_ImageEffectChain = new ImageEffectChain();
    m_ImageEffectChain->add(new Bloom());
    m_ImageEffectChain->add(new Tonemapping());
    // m_ImageEffectChain->add(new GaussianBlur());     // TODO: runtime toggle

    // ImGui
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    m_ImGuiIO = &ImGui::GetIO();

    // Lock mouse movement by default to enable free look + movement
    Input::SetMouseCursorState(MouseCursorState::Locked);

    // Load scene.
    m_CurrentScene = new Scene();

    // GUI windows
    m_DebugOverlayWindow = new DebugOverlayWindow();
    m_TexPickerWindow = new TexturePickerWindow(&HandleSelectedNewTexture);
}

GameContainer::~GameContainer()
{
    if (m_Instance == this)
        m_Instance = nullptr;

    delete m_ScreenBuffer;
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

    // Render scene into FP framebuffer for HDR.
    glBindFramebuffer(GL_FRAMEBUFFER, m_ScreenBuffer->id());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_CurrentScene)
        m_CurrentScene->draw();

    // Display framebuffer contents after running through post-processing chain.
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
    m_ImageEffectChain->render(m_ScreenBuffer);

    onGUI();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
}

void GameContainer::HandleSelectedNewTexture(const std::string& path)
{
    if (!m_Instance)
        return;

    m_Instance->m_CurrentScene->setNewTexture(path);
}
