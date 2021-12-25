#include "DebugWindow.h"

enum class DebugWindow::Tab
{
    TextureList,
    RenderingStats
};
const char* DebugWindow::TAB_NAMES[TAB_COUNT]
{
    "Texture List",
    "Rendering Stats"
};
const ImVec4 DebugWindow::TAB_SELECTED_COLOR = ImVec4(0.05f, 0.55f, 0.175f, 1.0f);


DebugWindow::DebugWindow(const char* windowName) : m_WindowOpened(true), m_WindowName(windowName), m_Tab(Tab::TextureList)
{
}

DebugWindow::~DebugWindow()
{
    clearTextures();
}

bool DebugWindow::isOpen() const
{
    return m_WindowOpened;
}

void DebugWindow::draw()
{
    if (!m_WindowOpened)
        return;

    float bgOpacity = m_Tab == Tab::TextureList ? 1.0f : 0.8f;
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.04f, 0.06f, bgOpacity));
    if (ImGui::Begin(m_WindowName, &m_WindowOpened))
    {
        // Tab-switching buttons aligned in a row
        size_t selectedTabIndex = static_cast<size_t>(m_Tab);
        for (size_t i = 0; i < TAB_COUNT; i++)
        {
            if (i != 0)
                ImGui::SameLine();

            if (i == selectedTabIndex)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, TAB_SELECTED_COLOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, TAB_SELECTED_COLOR);
            }

            if (ImGui::Button(TAB_NAMES[i]))
                m_Tab = static_cast<Tab>(i);

            if (i == selectedTabIndex)
                ImGui::PopStyleColor(/*count=*/ 2);
        }

        ImGui::Dummy(ImVec2(0, 3));

        const ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar;
        ImGui::BeginChild("Debug_Contents", /*size=*/ ImVec2(0, 0), /*border=*/ false, flags);
        switch (m_Tab)
        {
            case Tab::TextureList:
                drawTextureList();
                break;
            case Tab::RenderingStats:
                drawRenderingStats();
                break;
        }
        ImGui::EndChild();
    }
    ImGui::End();
    ImGui::PopStyleColor(/*count=*/ 1);
}

void DebugWindow::setOpen(bool open)
{
    m_WindowOpened = open;
}

void DebugWindow::addTexture(Texture* tex, const char* label, bool flipY, ElementSizeMode sizeMode)
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

void DebugWindow::removeTexture(Texture* tex)
{
    auto iter = std::find_if(
        m_Elements.begin(),
        m_Elements.end(),
        [&t = tex] (const Element& target) -> bool { return t == target.texture; }
    );

    if (iter != m_Elements.end())
        m_Elements.erase(iter);
}

void DebugWindow::clearTextures()
{
    m_Elements.clear();
}

void DebugWindow::drawTextureList()
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

void DebugWindow::drawRenderingStats()
{
    ImGui::Text("============ BASIC ============");
    ImGui::Text("Draw Calls: %d", GlobalStats::GetDrawCalls());
    ImGui::Text("Vertex Count: %d", GlobalStats::GetVertexCount());
    ImGui::Text("Triangle Count: %d", GlobalStats::GetIndexCount() / 3);
    ImGui::Text("Shader Calls: %d", GlobalStats::GetShaderCalls());
    ImGui::Text("Framebuffer Bind Calls: %d", GlobalStats::GetFramebufferBindCalls());
}