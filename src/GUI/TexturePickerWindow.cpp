#include "TexturePickerWindow.h"

TexturePickerWindow::TexturePickerWindow(std::function<void(const std::string&)> selectCallback) : m_WindowOpened(true)
{
    std::string baseDirPath = "res\\textures";

    if (fs::exists(baseDirPath) && fs::is_directory(baseDirPath))
    {
        for (const fs::directory_entry& entry : fs::recursive_directory_iterator(baseDirPath))
        {
            if (!entry.is_regular_file())
                continue;

            fs::path filePath = entry.path();

            if (filePath.extension() == ".png")
            {
                m_TexturePaths.push_back(filePath);
            }
        }
    }

    m_TexSelectCallback = selectCallback;
}

void TexturePickerWindow::draw()
{
    if (!m_WindowOpened)
        return;

    if (ImGui::Begin("Texture Picker", &m_WindowOpened))
    {
        ImGui::Text("%d textures", m_TexturePaths.size());
        ImGui::BeginChild("TextureList");
        {
            for (fs::path& path : m_TexturePaths)
            {
                std::string filename = path.filename().string();

                if (ImGui::Button(filename.c_str()) && m_TexSelectCallback)
                {
                    m_TexSelectCallback(path.string());
                }
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
}
