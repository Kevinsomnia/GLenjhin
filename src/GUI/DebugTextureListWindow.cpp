#include "GUIWindows.h"

DebugTextureListWindow::DebugTextureListWindow(const char* windowName) : m_WindowOpened(true), m_WindowName(windowName)
{
}

DebugTextureListWindow::~DebugTextureListWindow()
{
    clear();
}

bool DebugTextureListWindow::isOpen() const
{
    return m_WindowOpened;
}

void DebugTextureListWindow::draw()
{
    if (!m_WindowOpened)
        return;

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.05f, 1.0f));
    if (ImGui::Begin(m_WindowName, &m_WindowOpened, ImGuiWindowFlags_HorizontalScrollbar))
    {
        for (const Element& e : m_Elements)
        {
            Texture* tex = e.texture;

            if (!tex || tex->id() == NULL)
                continue;

            if (e.label)
            {
                ImGui::Text(e.label);
                ImGui::SameLine();
            }

            ImGui::Text("(%dx%d)", tex->width(), tex->height());
            ImVec2 size = ImVec2(static_cast<float>(tex->width()), static_cast<float>(tex->height()));
            ImVec2 uv0 = e.flipY ? ImVec2(0.0f, 1.0f) : ImVec2(0.0f, 0.0f);
            ImVec2 uv1 = e.flipY ? ImVec2(1.0f, 0.0f) : ImVec2(1.0f, 1.0f);
            ImGui::Image((ImTextureID)(uint64_t)tex->id(), size, uv0, uv1);

            ImGui::Dummy(ImVec2(0.0f, 20.0f));
        }
    }
    ImGui::End();
    ImGui::PopStyleColor(/*count=*/ 1);
}

void DebugTextureListWindow::setOpen(bool open)
{
    m_WindowOpened = open;
}

void DebugTextureListWindow::add(Texture* tex, const char* label, bool flipY)
{
    Element e = Element();
    e.texture = tex;
    e.label = label;
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

