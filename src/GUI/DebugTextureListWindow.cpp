#include "GUIWindows.h"

DebugTextureListWindow::DebugTextureListWindow(const char* windowName) : m_WindowName(windowName)
{
}

DebugTextureListWindow::~DebugTextureListWindow()
{
    clear();
}

void DebugTextureListWindow::draw()
{
    if (ImGui::Begin(m_WindowName, NULL, ImGuiWindowFlags_HorizontalScrollbar))
    {
        for (const Element& e : m_Elements)
        {
            Texture* tex = e.texture;

            if (!tex || tex->id() == NULL)
                continue;

            ImVec2 size = ImVec2(static_cast<float>(tex->width()), static_cast<float>(tex->height()));
            ImVec2 uv0 = e.flipY ? ImVec2(0.0f, 1.0f) : ImVec2(0.0f, 0.0f);
            ImVec2 uv1 = e.flipY ? ImVec2(1.0f, 0.0f) : ImVec2(1.0f, 1.0f);
            ImGui::Image(reinterpret_cast<ImTextureID>(tex->id()), size, uv0, uv1);
        }
    }
    ImGui::End();
}

void DebugTextureListWindow::add(Texture* tex, bool flipY)
{
    Element e = Element();
    e.texture = tex;
    e.flipY = flipY;
    m_Elements.push_back(e);
}

void DebugTextureListWindow::remove(Texture* tex)
{
    auto iter = std::find_if(
        m_Elements.begin(),
        m_Elements.end(),
        [&t = tex] (const Element& target) -> bool { return t == target.texture; }
    );

    if (iter != m_Elements.end())
        m_Elements.erase(iter);
}

void DebugTextureListWindow::clear()
{
    m_Elements.clear();
}

