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

    // Frame/screen buffers
    glGenFramebuffers(1, &m_ScreenFboID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ScreenFboID);

    // Color buffer
    m_ColorBuffer = new BufferTexture(1600, 900, TextureFormat::RGBAHalf);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffer->id(), 0);
    // Depth buffer
    uint32_t depthRboID;
    glGenRenderbuffers(1, &depthRboID);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRboID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1600, 900);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRboID);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cerr << "HDR framebuffer (RGBAHalf) failed to initialize. This graphics device doesn't support HDR." << endl;
        glDeleteFramebuffers(1, &m_ScreenFboID);
        m_ScreenFboID = NULL;
        delete m_ColorBuffer;
        m_ColorBuffer = nullptr;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, NULL);

    // Image effects
    m_ImageEffectChain = new ImageEffectChain();
    m_ImageEffectChain->add(new Tonemapping());

    // TODO: optimize this abomination
    // for (int i = 0; i < 16; i++)
    //     m_ImageEffectChain->add(new GaussianBlur());

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

    glDeleteFramebuffers(1, &m_ScreenFboID);
    m_ScreenFboID = NULL;

    if (m_ColorBuffer)
        delete m_ColorBuffer;

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
    // Render scene into FP framebuffer for HDR.
    glBindFramebuffer(GL_FRAMEBUFFER, m_ScreenFboID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_CurrentScene)
        m_CurrentScene->draw();

    // Display framebuffer contents after running through post-processing chain.
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
    m_ImageEffectChain->render(m_ColorBuffer);

    // GUI overlay
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
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
