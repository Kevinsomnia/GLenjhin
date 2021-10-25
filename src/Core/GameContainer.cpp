#include "GameContainer.h"

GameContainer::GameContainer(GLFWwindow* window)
    : m_MainWindow(window), m_CurrentScene(nullptr), m_ImGuiIO(nullptr), m_DisplayDebugOverlay(true),
    m_FrameCountInLastSecond(0), m_LastFPSRecordTime(0.0), m_FPS(0.0f), m_VSync(true)
{
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    // Enable back face culling
    glEnable(GL_CULL_FACE);

    // Core
    Time::Init();
    Input::Init(window);
    MeshPrimitives::Init();

    // ImGui
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    m_ImGuiIO = &ImGui::GetIO();

    // Lock mouse movement by default to enable free look + movement
    Input::SetMouseCursorState(MouseCursorState::Locked);

    // Load scene.
    m_CurrentScene = new Scene();
}

GameContainer::~GameContainer()
{
    delete m_CurrentScene;
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
        m_DisplayDebugOverlay = !m_DisplayDebugOverlay;
        m_FPS = 0.0f;
        m_LastFPSRecordTime = Time::GetTime();
        m_FrameCountInLastSecond = 0;
    }
}

void GameContainer::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5f, 0.7f, 0.825f, 1.0f);

    if (m_CurrentScene)
        m_CurrentScene->draw();

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
    if (!m_DisplayDebugOverlay)
        return;

    m_FrameCountInLastSecond++;
    double t = Time::GetTime();
    double elapsedTime = t - m_LastFPSRecordTime;

    if (elapsedTime >= 1.0)
    {
        m_FPS = (m_FrameCountInLastSecond / static_cast<float>(elapsedTime));
        m_FrameCountInLastSecond = 0;
        m_LastFPSRecordTime = t;
    }
}

void GameContainer::onGUI()
{
    handleDebugOverlay();
}

void GameContainer::handleDebugOverlay()
{
    // Drawing
    if (m_DisplayDebugOverlay)
    {
        const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar |
                                            (ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize) |
                                            ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoCollapse |
                                            (ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus);

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Debug", &m_DisplayDebugOverlay, windowFlags);
        {
            // FPS counter
            float frameTime = (m_FPS > 0.0f) ? 1000.0f / m_FPS : 0.0f;
            ImGui::Text("%.2f FPS (%.2f ms)", m_FPS, frameTime);

            ImGui::Checkbox("V-Sync", &m_VSync);
        }
        ImGui::End();
    }

    // Other logic
    glfwSwapInterval(m_VSync);
}
