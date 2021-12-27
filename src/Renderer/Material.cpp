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
    Shader::SetActiveID(NULL);
}

float Material::getFloat(const string& uniformName) const
{
    auto iter = m_UniformFloats.find(uniformName);

    if (iter != m_UniformFloats.end())
        return (*iter).second;

    return 0.0f;
}

Vector2 Material::getVector2(const string& uniformName) const
{
    auto iter = m_UniformVec2.find(uniformName);

    if (iter != m_UniformVec2.end())
        return (*iter).second;

    return Vector2::zero;
}

Vector3 Material::getVector3(const string& uniformName) const
{
    auto iter = m_UniformVec3.find(uniformName);

    if (iter != m_UniformVec3.end())
        return (*iter).second;

    return Vector3::zero;
}

Vector4 Material::getVector4(const string& uniformName) const
{
    auto iter = m_UniformVec4.find(uniformName);

    if (iter != m_UniformVec4.end())
        return (*iter).second;

    return Vector4::zero;
}

Color Material::getColor(const string& uniformName) const
{
    return Color(getVector4(uniformName));
}

Matrix4x4 Material::getMatrix4x4(const string& uniformName) const
{
    auto iter = m_UniformMat4.find(uniformName);

    if (iter != m_UniformMat4.end())
        return (*iter).second;

    return Matrix4x4::identity;
}

Texture* Material::getTexture(const string& uniformName) const
{
    auto iter = m_UniformTex.find(uniformName);

    if (iter != m_UniformTex.end())
        return (*iter).second;

    return nullptr;
}

void Material::setFloat(const string& uniformName, float v)
{
    m_UniformFloats[uniformName] = v;
}

void Material::setVector2(const string& uniformName, const Vector2& v)
{
    m_UniformVec2[uniformName] = v;
}

void Material::setVector3(const string& uniformName, const Vector3& v)
{
    m_UniformVec3[uniformName] = v;
}

void Material::setVector4(const string& uniformName, const Vector4& v)
{
    m_UniformVec4[uniformName] = v;
}

void Material::setColor(const string& uniformName, const Color& c)
{
    m_UniformVec4[uniformName] = Vector4(c);
}

void Material::setMatrix4x4(const string& uniformName, const Matrix4x4& mat)
{
    m_UniformMat4[uniformName] = mat;
}

void Material::setTexture(const string& uniformName, Texture* tex)
{
    m_UniformTex[uniformName] = tex;
}

void Material::updateUniforms() const
{
    // We will need to bind the shader temporarily to retrieve the location, since another shader
    // might be already bound when we're attempting to update these uniforms.
    GLint prevShaderID = Shader::GetActiveID();
    m_Shader->use();

    // Vector1 (float) to Vector4
    for (const auto& pair : m_UniformFloats)
    {
        int id = getShaderUniformLocation(pair.first);

        if (id != -1)
            glUniform1f(id, pair.second);
    }
    for (const auto& pair : m_UniformVec2)
    {
        int id = getShaderUniformLocation(pair.first);

        if (id != -1)
            glUniform2fv(id, 1, pair.second);
    }
    for (const auto& pair : m_UniformVec3)
    {
        int id = getShaderUniformLocation(pair.first);

        if (id != -1)
            glUniform3fv(id, 1, pair.second);
    }
    for (const auto& pair : m_UniformVec4)
    {
        int id = getShaderUniformLocation(pair.first);

        if (id != -1)
            glUniform4fv(id, 1, pair.second);
    }

    // Matrices
    for (const auto& pair : m_UniformMat4)
    {
        int id = getShaderUniformLocation(pair.first);

        if (id != -1)
            glUniformMatrix4fv(id, 1, GL_FALSE, pair.second);
    }

    // Texture units
    uint32_t unitIndex = 0;
    for (const auto& pair : m_UniformTex)
    {
        int id = getShaderUniformLocation(pair.first);

        if (id != -1)
        {
            if (pair.second)
            {
                pair.second->bind(unitIndex);
            }
            else
            {
                glBindTextureUnit(unitIndex, GL_NONE);
            }

            glUniform1i(id, unitIndex);
            unitIndex++;
        }
    }

    // Restore to previous shader program since we're done retrieving the uniform location.
    Shader::SetActiveID(prevShaderID);
}

int Material::getShaderUniformLocation(const string& name) const
{
    if (!m_Shader)
    {
        cerr << "Tried to get uniform location, but there was no shader assigned to this material!" << endl;
        return -1;
    }

    int uniformId = glGetUniformLocation(m_Shader->id(), name.c_str());

    if (uniformId == GL_INVALID_OPERATION || uniformId == GL_INVALID_VALUE)
        return -1;

    return uniformId;
}
