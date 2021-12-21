#pragma once

#include <filesystem>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "imgui.h"
#include "../Renderer/Texture/Texture.h"

namespace fs = std::filesystem;

class DebugTextureListWindow
{
public:
    enum class ElementSizeMode
    {
        Original,                   // Size is left untouched.
        ConstrainToWindowWidth,     // Size is only reduced only if the width exceeds the window width, while maintaining aspect ratio.
        FitToWindowWidth,           // Size is forced to the window width (constrained if too large, expanded if too small), maintaining aspect ratio.
    };

    DebugTextureListWindow(const char* windowName);
    ~DebugTextureListWindow();
    bool isOpen() const;
    void draw();
    void setOpen(bool open);
    void add(Texture* tex, const char* label, bool flipY = false, ElementSizeMode sizeMode = ElementSizeMode::Original);
    void remove(Texture* tex);
    void clear();

private:
    struct Element
    {
        Texture* texture;
        const char* label;
        bool flipY;
        ElementSizeMode sizeMode;
    };

    bool m_WindowOpened;
    const char* m_WindowName;
    std::vector<Element> m_Elements;
};
