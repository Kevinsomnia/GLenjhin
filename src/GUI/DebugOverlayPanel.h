#pragma once

#include <filesystem>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "DebugWindow.h"

namespace fs = std::filesystem;
using std::cout;
using std::endl;

class DebugOverlayPanel
{
public:
    DebugOverlayPanel() = delete;
    DebugOverlayPanel(DebugWindow* debugWindow);
    void draw();
    bool visible();
    void toggleVisible();
    void setFPS(float fps);
private:
    bool m_Visible;
    float m_FPS;
    bool m_VSync;
    DebugWindow* m_DebugWindow;

    void updateVSync();
};
