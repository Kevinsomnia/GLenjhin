#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <unordered_map>
#include <GL/glew.h>
#include "../Math/Matrix4x4.h"
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
	void setVector(const string& uniformName, const Vector3& v);
	void setMatrix(const string& uniformName, const Matrix4x4& mat);
private:
	Shader* m_Shader;

	// uniform mappings: uniform ID -> value
	std::unordered_map<GLint, Vector3> m_UniformVec3;
	std::unordered_map<GLint, Matrix4x4> m_UniformMat4;

	int getShaderUniformLocation(const string& name) const;
	void setUniforms() const;
};

#endif // MATERIAL_H
