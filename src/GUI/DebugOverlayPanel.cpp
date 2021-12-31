#include "DebugOverlayPanel.h"

DebugOverlayPanel::DebugOverlayPanel(DebugWindow* debugWindow)
    : m_Visible(true), m_FPS(0.0f), m_VSync(true), m_DebugWindow(debugWindow)
{
    updateVSync();
}

void DebugOverlayPanel::draw()
{
    if (m_Visible)
    {
        const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar |
                                            (ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize) |
                                            ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoCollapse |
                                            (ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Debug", NULL, windowFlags);
        {
            // FPS counter
            float frameTime = (m_FPS > 0.0f) ? 1000.0f / m_FPS : 0.0f;
            ImGui::Text("%.2f FPS (%.2f ms)", m_FPS, frameTime);

            if (ImGui::Checkbox("V-Sync", &m_VSync))
                updateVSync();

            if (m_DebugWindow && !m_DebugWindow->isOpen() && ImGui::Button("Debug/Stats"))
            {
                m_DebugWindow->setOpen(true);
            }
        }
        ImGui::End();
        ImGui::PopStyleColor(/*count=*/ 1);
    }
}

bool DebugOverlayPanel::visible()
{
    return m_Visible;
}

void DebugOverlayPanel::toggleVisible()
{
    m_Visible = !m_Visible;
    m_FPS = 0.0f;
}

void DebugOverlayPanel::setFPS(float fps)
{
    m_FPS = fps;
}

void DebugOverlayPanel::updateVSync()
{
    glfwSwapInterval(m_VSync);
}
