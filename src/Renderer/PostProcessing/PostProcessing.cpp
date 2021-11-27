#include "PostProcessing.h"

ImageEffect::ImageEffect(const std::string& shaderPath)
{
    m_Shader = new Shader(shaderPath);
    m_Material = new Material(m_Shader);
    m_Triangle = new FullscreenTriangle(m_Material);
}

ImageEffect::~ImageEffect()
{
    delete m_Shader;
    delete m_Material;
    delete m_Triangle;
}

void ImageEffect::render(Texture* src)
{
    m_Material->setTexture("u_MainTex", src);
    m_Material->bind();
    m_Triangle->draw();
}
