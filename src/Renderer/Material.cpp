#include "Material.h"

Material::Material(Shader* shader) : m_Shader(shader)
{
}

Material::~Material()
{
}

void Material::bind() const
{
    if (m_Shader)
    {
        m_Shader->use();
        setUniforms();
    }
}

void Material::unbind() const
{
    glUseProgram(NULL);
}

void Material::setVector(const string& uniformName, const Vector3& v)
{
    int uniformId = getShaderUniformLocation(uniformName);

    if (uniformId != -1)
        m_UniformVec3[uniformId] = v;
}

void Material::setMatrix(const string& uniformName, const Matrix4x4& mat)
{
    int uniformId = getShaderUniformLocation(uniformName);

    if (uniformId != -1)
        m_UniformMat4[uniformId] = mat;
}

void Material::setTexture(const string& uniformName, Texture2D* tex)
{
    int uniformId = getShaderUniformLocation(uniformName);

    if (uniformId != -1)
        m_UniformTex2D[uniformId] = tex;
}

int Material::getShaderUniformLocation(const string& name) const
{
    if (!m_Shader)
    {
        cerr << "Tried to get uniform location, but there was no shader assigned to this material!" << endl;
        return -1;
    }

    uint32_t programId = m_Shader->getShaderId();
    glUseProgram(programId);
    int uniformId = glGetUniformLocation(programId, name.c_str());

    if (uniformId == GL_INVALID_OPERATION || uniformId == GL_INVALID_VALUE)
        return -1;

    return uniformId;
}

void Material::setUniforms() const
{
    for (const auto& pair : m_UniformVec3)
        glUniform3fv(pair.first, 1, pair.second);
    for (const auto& pair : m_UniformMat4)
        glUniformMatrix4fv(pair.first, 1, GL_FALSE, pair.second);

    uint32_t unitIndex = 0;
    for (const auto& pair : m_UniformTex2D)
    {
        pair.second->bind(unitIndex);
        glUniform1i(pair.first, unitIndex);
        unitIndex++;
    }
}
