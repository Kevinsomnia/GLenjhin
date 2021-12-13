#pragma once

#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include "../Math/Matrix4x4.h"
#include "Texture/Texture.h"
#include "Shader.h"

using std::cout;
using std::cerr;
using std::endl;

class Material
{
public:
    Material(Shader* shader);
    ~Material();
    void bind() const;
    void unbind() const;
    Vector4 getVector4(const string& uniformName) const;
    Color getColor(const string& uniformName) const;
    Texture* getTexture(const string& uniformName) const;
    void setFloat(const string& uniformName, float v);
    void setVector2(const string& uniformName, const Vector2& v);
    void setVector3(const string& uniformName, const Vector3& v);
    void setVector4(const string& uniformName, const Vector4& v);
    void setColor(const string& uniformName, const Color& c);
    void setMatrix(const string& uniformName, const Matrix4x4& mat);
    void setTexture(const string& uniformName, Texture* tex);
private:
    Shader* m_Shader;

    std::unordered_map<string, float> m_UniformFloats;
    std::unordered_map<string, Vector2> m_UniformVec2;
    std::unordered_map<string, Vector3> m_UniformVec3;
    std::unordered_map<string, Vector4> m_UniformVec4;
    std::unordered_map<string, Matrix4x4> m_UniformMat4;
    std::unordered_map<string, Texture*> m_UniformTex;

    void updateUniforms() const;
    int getShaderUniformLocation(const string& name) const;
};
