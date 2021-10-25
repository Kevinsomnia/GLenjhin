#ifndef GAMECONTAINER_H
#define GAMECONTAINER_H

#define IMGUI_IMPL_OPENGL_ES3

#include <iostream>
#include "../Interaction/Input.h"
#include "../Renderer/MeshPrimitives.h"
#include "Scene.h"
#include "Time.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using std::cout;
using std::endl;

class GameContainer
{
public:
    GameContainer(GLFWwindow* window);
    ~GameContainer();

    // Behavior
    void update(double deltaTime);
    void render();
    inline void handleMouseCursorState();
    inline void handleFPSCounter();

    // GUI
    void onGUI();
    void drawDebugOverlay();
private:
    GLFWwindow* m_MainWindow;
    Scene* m_CurrentScene;
    ImGuiIO* m_ImGuiIO;
    bool m_DisplayDebugOverlay;
    uint32_t m_FrameCountInLastSecond;
    double m_LastFPSRecordTime;
    float m_FPS;
};

#endif // GAMECONTAINER_H
