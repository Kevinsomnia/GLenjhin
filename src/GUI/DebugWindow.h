#pragma once

#include <filesystem>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "imgui.h"
#include "../Renderer/GlobalStats.h"
#include "../Renderer/Texture/Texture.h"

namespace fs = std::filesystem;

class DebugWindow
{
public:
    enum class ElementSizeMode
    {
        Original,                   // Size is left untouched.
        ConstrainToWindowWidth,     // Size is only reduced only if the width exceeds the window width, while maintaining aspect ratio.
        FitToWindowWidth,           // Size is forced to the window width (constrained if too large, expanded if too small), maintaining aspect ratio.
    };

    DebugWindow(const char* windowName);
    ~DebugWindow();
    bool isOpen() const;
    void draw();
    void setOpen(bool open);
    void addTexture(Texture* tex, const char* label, bool flipY = false, ElementSizeMode sizeMode = ElementSizeMode::Original);
    void removeTexture(Texture* tex);
    void clearTextures();

private:
    enum class Tab;
    static const size_t TAB_COUNT = 2;
    static const char* TAB_NAMES[TAB_COUNT];
    static const ImVec4 TAB_SELECTED_COLOR;

    struct Element
    {
        Texture* texture;
        const char* label;
        bool flipY;
        ElementSizeMode sizeMode;
    };

    bool m_WindowOpened;
    Tab m_Tab;
    const char* m_WindowName;
    std::vector<Element> m_Elements;

    void drawTextureList();
    void drawRenderingStats();
};
