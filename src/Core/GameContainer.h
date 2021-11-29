#ifndef GAMECONTAINER_H
#define GAMECONTAINER_H

#define IMGUI_IMPL_OPENGL_ES3

#include <iostream>
#include "../GUI/GUIWindows.h"
#include "../Interaction/Input.h"
#include "../Renderer/MeshPrimitives.h"
#include "../Renderer/PostProcessing/Bloom.h"
#include "../Renderer/PostProcessing/GaussianBlur.h"
#include "../Renderer/PostProcessing/GlobalFog.h"
#include "../Renderer/PostProcessing/Tonemapping.h"
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
    inline void handleCameraMovement(double deltaTime);
    inline void handleMouseCursorState();
    inline void handleFPSCounter();
    inline void onGUI();
private:
    static GameContainer* m_Instance;

    GLFWwindow* m_MainWindow;
    Scene* m_CurrentScene;
    Camera* m_MainCamera;
    ImGuiIO* m_ImGuiIO;
    uint32_t m_FrameCountInLastSecond;
    double m_LastFPSRecordTime;

    TexturePickerWindow* m_TexPickerWindow;
    DebugTextureListWindow* m_DebugTexturesWindow;
    DebugOverlayWindow* m_DebugOverlayWindow;

    static void HandleSelectedNewTexture(const std::string& path);

    inline Vector3 getMoveAxis() const;
};

#endif // GAMECONTAINER_H
