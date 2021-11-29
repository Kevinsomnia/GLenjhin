#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <iostream>
#include <GL/glew.h>
#include "Lighting/Lighting.h"
#include "Material.h"
#include "Mesh.h"

using std::cout;
using std::endl;

class MeshRenderer
{
public:
    MeshRenderer(Mesh* mesh, Material* material);
    ~MeshRenderer();
    inline void drawMeshDirect() const;
    void draw(const Vector3& cameraPos, const Matrix4x4& vp, const Matrix4x4& model, const std::vector<Light*>& lights) const;

    Mesh* getMesh() const { return m_Mesh; }
    Material* getMaterial() const { return m_Material; }
private:
    Mesh* m_Mesh;
    Material* m_Material;
};

#endif // MESHRENDERER_H
