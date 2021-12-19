#pragma once

#define IMGUI_IMPL_OPENGL_ES3

#include "../GUI/DebugOverlayWindow.h"
#include "../GUI/DebugTextureListWindow.h"
#include "../GUI/TexturePickerWindow.h"
#include "../IO/ModelLib.h"
#include "../Renderer/ImageEffects/Deferred/SSAO.h"
#include "../Renderer/ImageEffects/PostProcess/Bloom.h"
#include "../Renderer/ImageEffects/PostProcess/FXAA.h"
#include "../Renderer/ImageEffects/PostProcess/GaussianBlur.h"
#include "../Renderer/ImageEffects/PostProcess/GlobalFog.h"
#include "../Renderer/ImageEffects/PostProcess/SunShafts.h"
#include "../Renderer/ImageEffects/PostProcess/Tonemapping.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using std::cout;
using std::endl;
using ModelLib::ModelImporter;

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
    Vector3 m_SmoothedCamMoveDir;
    uint32_t m_FrameCountInLastSecond;
    double m_LastFPSRecordTime;

    TexturePickerWindow* m_TexPickerWindow;
    DebugTextureListWindow* m_DebugTexturesWindow;
    DebugOverlayWindow* m_DebugOverlayWindow;

    static void HandleSelectedNewTexture(const std::string& path);

    inline Vector3 getMoveAxis() const;
};
