#ifndef GUI_WINDOWS_H
#define GUI_WINDOWS_H

#include <filesystem>
#include <functional>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "imgui.h"
#include "../Renderer/Texture/Texture.h"

namespace fs = std::filesystem;
using std::cout;
using std::endl;

// Declaring classes that will be accessed before their implementation.
class DebugTextureListWindow;


class DebugOverlayWindow
{
public:
    DebugOverlayWindow() = delete;
    DebugOverlayWindow(DebugTextureListWindow* debugBuffersWindow);
    void draw();
    bool getVisible();
    void toggleVisible();
    void setFPS(float fps);
private:
    bool m_Visible;
    float m_FPS;
    bool m_VSync;
    DebugTextureListWindow* m_DebugBuffersWindow;

    void updateVSync();
};

class TexturePickerWindow
{
public:
    TexturePickerWindow(std::function<void(const std::string&)> selectCallback);
    void draw();
private:
    bool m_WindowOpened;
    std::vector<fs::path> m_TexturePaths;
    std::function<void(const std::string&)> m_TexSelectCallback;
};

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

#endif GUI_WINDOWS_H
