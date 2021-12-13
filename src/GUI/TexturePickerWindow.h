#pragma once

#include <filesystem>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "imgui.h"

namespace fs = std::filesystem;

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
