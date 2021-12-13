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
    DebugTextureListWindow(const char* windowName);
    ~DebugTextureListWindow();
    bool isOpen() const;
    void draw();
    void setOpen(bool open);
    void add(Texture* tex, const char* label, bool flipY = false);
    void remove(Texture* tex);
    void clear();
private:
    struct Element
    {
        Texture* texture;
        const char* label;
        bool flipY;
    };

    bool m_WindowOpened;
    const char* m_WindowName;
    std::vector<Element> m_Elements;
};
