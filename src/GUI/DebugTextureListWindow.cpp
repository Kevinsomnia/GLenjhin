#include "DebugTextureListWindow.h"

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
    if (ImGui::Begin(m_WindowName, &m_WindowOpened, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar))
    {
        float windowWidth = Math::Clamp(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x, 1.0f, 100000.0f);

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

            // Append the texture size to the label
            ImGui::Text("(%dx%d)", tex->width(), tex->height());

            Vector2 texSize = tex->size();
            float texAspect = (texSize.y > 0.5f) ? texSize.x / texSize.y : 1.0f;

            ImVec2 imgSize;

            switch (e.sizeMode)
            {
                case ElementSizeMode::ConstrainToWindowWidth:
                    imgSize = (texSize.x > windowWidth) ? ImVec2(windowWidth, windowWidth / texAspect) : ImVec2(texSize.x, texSize.y);
                    break;
                case ElementSizeMode::FitToWindowWidth:
                    imgSize = ImVec2(windowWidth, windowWidth / texAspect);
                    break;
                default:
                    imgSize = ImVec2(texSize.x, texSize.y);
                    break;
            }

            ImVec2 uv0 = e.flipY ? ImVec2(0.0f, 1.0f) : ImVec2(0.0f, 0.0f);
            ImVec2 uv1 = e.flipY ? ImVec2(1.0f, 0.0f) : ImVec2(1.0f, 1.0f);
            ImGui::Image((ImTextureID)(uint64_t)tex->id(), imgSize, uv0, uv1);

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

void DebugTextureListWindow::add(Texture* tex, const char* label, bool flipY, ElementSizeMode sizeMode)
{
    if (!tex)
        return;

    Element e = Element();
    e.texture = tex;
    e.label = label;
    e.flipY = flipY;
    e.sizeMode = sizeMode;
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

