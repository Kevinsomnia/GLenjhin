#ifndef GUI_WINDOWS_H
#define GUI_WINDOWS_H

#include <filesystem>
#include <functional>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "imgui.h"

namespace fs = std::filesystem;
using std::cout;
using std::endl;

class DebugOverlayWindow
{
public:
    DebugOverlayWindow();
    void draw();
    bool getVisible();
    void toggleVisible();
    void setFPS(float fps);
private:
    bool m_Visible;
    float m_FPS;
    bool m_VSync;

    void updateVSync();
};

class TexturePickerWindow
{
public:
    TexturePickerWindow(std::function<void(const std::string&)> selectCallback);
    void draw();
private:
    std::vector<fs::path> m_TexturePaths;
    std::function<void(const std::string&)> m_TexSelectCallback;
};

#endif GUI_WINDOWS_H
