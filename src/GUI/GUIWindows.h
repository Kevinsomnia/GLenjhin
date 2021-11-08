#ifndef GUI_WINDOWS_H
#define GUI_WINDOWS_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"

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

#endif GUI_WINDOWS_H
