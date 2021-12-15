#include "Skybox.h"

Skybox::Skybox(const std::string& texturePath)
{
    m_Texture = new Texture2D(texturePath, /*generateMipmaps=*/ false, /*readable=*/ false, /*sRGB=*/ true);
    m_Material = new Material(new Shader("res\\shaders\\Skybox.glsl"));
    m_Material->setTexture("u_MainTex", m_Texture);
    m_Triangle = new FullscreenTriangle(m_Material, /*depthTest=*/ true);
}

Skybox::~Skybox()
{
    delete m_Triangle;
    delete m_Material;
    delete m_Texture;
}

void Skybox::draw(const Matrix4x4& vp)
{
    m_Material->setMatrix4x4("u_VP", vp);
    m_Triangle->draw();
}
