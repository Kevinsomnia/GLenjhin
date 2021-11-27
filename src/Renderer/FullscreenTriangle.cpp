#include "FullscreenTriangle.h"

uint32_t FullscreenTriangle::VAO_ID = 0;

FullscreenTriangle::FullscreenTriangle(Material* mat) : m_Material(mat)
{
    std::array<float, 3*2> positions = {
        -1.0f, -1.0f,
        3.0f, -1.0f,
        -1.0f, 3.0f
    };

    // Create simple VAO/VBO containing only the vertex position.
    glGenVertexArrays(1, &VAO_ID);
    glBindVertexArray(VAO_ID);

    uint32_t vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
    glEnableVertexAttribArray(0);
}

FullscreenTriangle::~FullscreenTriangle()
{
}

void FullscreenTriangle::setMaterial(Material* mat)
{
    m_Material = mat;
}

void FullscreenTriangle::draw() const
{
    bool prevDepthTest = glIsEnabled(GL_DEPTH_TEST);

    if (prevDepthTest)
        glDisable(GL_DEPTH_TEST);

    if (m_Material)
        m_Material->bind();

    glBindVertexArray(VAO_ID);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    if (prevDepthTest)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}
