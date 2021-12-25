#include "Skybox.h"

Skybox::Skybox(const std::string& texturePath)
{
    m_Texture = new Texture2D(texturePath, /*generateMipmaps=*/ false, /*readable=*/ false, /*sRGB=*/ true);
    m_Material = new Material(new Shader("res\\shaders\\Skybox.glsl"));
    m_Material->setTexture("u_MainTex", m_Texture);
}

Skybox::~Skybox()
{
    delete m_Material;
    delete m_Texture;
}

void Skybox::draw(const Matrix4x4& vp)
{
    m_Material->setMatrix4x4("u_VP", vp);
    FullscreenTriangle::Draw(m_Material, /*depthTest=*/ true);
}
