#ifndef GAMECONTAINER_H
#define GAMECONTAINER_H

#define IMGUI_IMPL_OPENGL_ES3

#include <iostream>
#include "../GUI/GUIWindows.h"
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
    const double FPS_UPDATE_INTERVAL = 0.5;

public:
    GameContainer(GLFWwindow* window);
    ~GameContainer();

    // Behavior
    void update(double deltaTime);
    void render();
    inline void handleMouseCursorState();
    inline void handleFPSCounter();
    inline void onGUI();
private:
    GLFWwindow* m_MainWindow;
    Scene* m_CurrentScene;
    DebugOverlayWindow* m_DebugOverlayWindow;
    ImGuiIO* m_ImGuiIO;
    uint32_t m_FrameCountInLastSecond;
    double m_LastFPSRecordTime;
};

#endif // GAMECONTAINER_H
