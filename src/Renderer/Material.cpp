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
        updateUniforms();
    }
}

void Material::unbind() const
{
    glUseProgram(NULL);
}

Vector4 Material::getVector4(const string& uniformName) const
{
    int uniformId = getShaderUniformLocation(uniformName);

    if (uniformId == -1)
        return Vector4::zero;

    auto iter = m_UniformVec4.find(uniformId);

    if (iter == m_UniformVec4.end())
        return Vector4::zero;

    return (*iter).second;
}

Color Material::getColor(const string& uniformName) const
{
    return Color(getVector4(uniformName));
}

Texture* Material::getTexture(const string& uniformName) const
{
    int uniformId = getShaderUniformLocation(uniformName);

    if (uniformId == -1)
        return nullptr;

    auto iter = m_UniformTex.find(uniformId);

    if (iter == m_UniformTex.end())
        return nullptr;

    return (*iter).second;
}

void Material::setFloat(const string& uniformName, float v)
{
    int uniformId = getShaderUniformLocation(uniformName);

    if (uniformId != -1)
        m_UniformFloats[uniformId] = v;
}

void Material::setVector2(const string& uniformName, const Vector2& v)
{
    int uniformId = getShaderUniformLocation(uniformName);

    if (uniformId != -1)
        m_UniformVec2[uniformId] = v;
}

void Material::setVector3(const string& uniformName, const Vector3& v)
{
    int uniformId = getShaderUniformLocation(uniformName);

    if (uniformId != -1)
        m_UniformVec3[uniformId] = v;
}

void Material::setVector4(const string& uniformName, const Vector4& v)
{
    int uniformId = getShaderUniformLocation(uniformName);

    if (uniformId != -1)
        m_UniformVec4[uniformId] = v;
}

void Material::setColor(const string& uniformName, const Color& v)
{
    setVector4(uniformName, Vector4(v));
}

void Material::setMatrix(const string& uniformName, const Matrix4x4& mat)
{
    int uniformId = getShaderUniformLocation(uniformName);

    if (uniformId != -1)
        m_UniformMat4[uniformId] = mat;
}

void Material::setTexture(const string& uniformName, Texture* tex)
{
    int uniformId = getShaderUniformLocation(uniformName);

    if (uniformId != -1)
        m_UniformTex[uniformId] = tex;
}

void Material::updateUniforms() const
{
    for (const auto& pair : m_UniformFloats)
        glUniform1f(pair.first, pair.second);
    for (const auto& pair : m_UniformVec2)
        glUniform2fv(pair.first, 1, pair.second);
    for (const auto& pair : m_UniformVec3)
        glUniform3fv(pair.first, 1, pair.second);
    for (const auto& pair : m_UniformVec4)
        glUniform4fv(pair.first, 1, pair.second);

    for (const auto& pair : m_UniformMat4)
        glUniformMatrix4fv(pair.first, 1, GL_FALSE, pair.second);

    // TEXTURES
    uint32_t unitIndex = 0;
    for (const auto& pair : m_UniformTex)
    {
        if (pair.second)
        {
            pair.second->bind(unitIndex);
        }
        else
        {
            glBindTextureUnit(unitIndex, GL_NONE);
        }

        glUniform1i(pair.first, unitIndex);
        unitIndex++;
    }
}

int Material::getShaderUniformLocation(const string& name) const
{
    if (!m_Shader)
    {
        cerr << "Tried to get uniform location, but there was no shader assigned to this material!" << endl;
        return -1;
    }

    GLint prevShaderID;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prevShaderID);

    // We will need to bind the shader temporarily to retrieve the location, since another shader
    // might be already bound when we're attempting to retrieve this uniform.
    m_Shader->use();
    int uniformId = glGetUniformLocation(m_Shader->id(), name.c_str());

    // Restore to previous shader program since we're done retrieving the uniform location.
    glUseProgram(prevShaderID);

    if (uniformId == GL_INVALID_OPERATION || uniformId == GL_INVALID_VALUE)
        return -1;

    return uniformId;
}
