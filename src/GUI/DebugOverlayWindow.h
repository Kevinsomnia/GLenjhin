#pragma once

#include <filesystem>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "DebugTextureListWindow.h"

namespace fs = std::filesystem;
using std::cout;
using std::endl;

class DebugOverlayWindow
{
public:
    DebugOverlayWindow() = delete;
    DebugOverlayWindow(DebugTextureListWindow* debugBuffersWindow);
    void draw();
    bool getVisible();
    void toggleVisible();
    void setFPS(float fps);
private:
    bool m_Visible;
    float m_FPS;
    bool m_VSync;
    DebugTextureListWindow* m_DebugBuffersWindow;

    void updateVSync();
};
