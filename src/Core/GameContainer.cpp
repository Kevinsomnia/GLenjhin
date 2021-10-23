#include "GameContainer.h"

GameContainer::GameContainer(GLFWwindow* window) : m_MainWindow(window), m_CurrentScene(nullptr)
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

    // Load scene.
    m_CurrentScene = new Scene();
}

GameContainer::~GameContainer()
{
}

void GameContainer::update(double deltaTime)
{
    Time::Timestep(deltaTime);
    Input::Poll();

    if (m_CurrentScene)
        m_CurrentScene->update();
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


void GameContainer::onGUI()
{
}
