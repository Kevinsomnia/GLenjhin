#include "Skybox.h"

Skybox::Skybox(const std::string& texturePath)
{
    Shader* shader = new Shader("res\\shaders\\Skybox.glsl");
    m_Texture = new Texture2D(texturePath, false);
    m_Material = new Material(shader);
    m_Material->setTexture("u_MainTex", m_Texture);
    m_Triangle = new FullscreenTriangle(m_Material);
}

Skybox::~Skybox()
{
    delete m_Triangle;
    delete m_Material;
    delete m_Texture;
}

void Skybox::draw(const Matrix4x4& vp)
{
    m_Material->setMatrix("u_VP", vp);
    m_Triangle->draw();
}
