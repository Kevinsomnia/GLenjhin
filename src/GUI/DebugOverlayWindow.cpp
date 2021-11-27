#include "GUIWindows.h"

DebugOverlayWindow::DebugOverlayWindow() : m_Visible(true), m_FPS(0.0f), m_VSync(true)
{
    updateVSync();
}

void DebugOverlayWindow::draw()
{
    if (m_Visible)
    {
        const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar |
                                            (ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize) |
                                            ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoCollapse |
                                            (ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus);

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Debug", NULL, windowFlags);
        {
            // FPS counter
            float frameTime = (m_FPS > 0.0f) ? 1000.0f / m_FPS : 0.0f;
            ImGui::Text("%.2f FPS (%.2f ms)", m_FPS, frameTime);

            if (ImGui::Checkbox("V-Sync", &m_VSync))
                updateVSync();
        }
        ImGui::End();
    }
}

bool DebugOverlayWindow::getVisible()
{
    return m_Visible;
}

void DebugOverlayWindow::toggleVisible()
{
    m_Visible = !m_Visible;
    m_FPS = 0.0f;
}

void DebugOverlayWindow::setFPS(float fps)
{
    m_FPS = fps;
}

void DebugOverlayWindow::updateVSync()
{
    glfwSwapInterval(m_VSync);
}
