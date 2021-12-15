#include "ModelLib.h"

namespace ModelLib
{
    void Model::cleanup()
    {
        for (size_t i = 0; i < meshes.size(); i++)
            delete meshes[i];
        meshes.clear();
    }

    Assimp::Importer* ModelImporter::importerInstance = nullptr;

    void ModelImporter::Init()
    {
        if (importerInstance)
            return;

        importerInstance = new Assimp::Importer();
    }

    Model ModelImporter::LoadFBX(const std::string& filePath)
    {
        assert(importerInstance);

        auto benchStart = steady_clock::now();
        const aiScene* scene = importerInstance->ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipWindingOrder);

        if (!scene)
        {
            cerr << "Failed to load FBX: " << filePath << endl;
            cerr << importerInstance->GetErrorString() << endl;
            return Model();
        }

        Model result;

        if (scene->HasMeshes())
        {
            float fileScale;
            if (!scene->mMetaData->Get("UnitScaleFactor", fileScale))
                fileScale = 0.01f;  // Default scale.

            cout << "Using file scale " << fileScale << endl;
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            for (uint32_t i = 0; i < scene->mNumMeshes; i++)
            {
                Mesh* mesh = new Mesh();

                vertices.clear();
                indices.clear();

                aiMesh* importMesh = scene->mMeshes[i];
                bool hasNormals = importMesh->mNormals;
                bool hasUV0 = importMesh->HasTextureCoords(0);

                for (uint32_t i = 0; i < importMesh->mNumVertices; i++)
                {
                    const aiVector3D& importPos = importMesh->mVertices[i];
                    Vector3 position = Vector3(importPos.x, importPos.y, importPos.z) * fileScale;
                    Vector3 normal;
                    Vector2 uv0;

                    if (hasNormals)
                    {
                        const aiVector3D& importNormal = importMesh->mNormals[i];
                        normal = Vector3(importNormal.x, importNormal.y, importNormal.z);
                    }
                    else
                    {
                        normal = position.getNormalized();
                    }

                    if (hasUV0)
                    {
                        const aiVector3D& importUV = importMesh->mTextureCoords[0][i];
                        uv0 = Vector2(importUV.x, importUV.y);
                    }
                    else
                    {
                        uv0 = Vector2::zero;
                    }

                    vertices.push_back(Vertex(position, normal, uv0));
                }

                for (uint32_t i = 0; i < importMesh->mNumFaces; i++)
                {
                    const aiFace& face = importMesh->mFaces[i];

                    for (uint32_t j = 0; j < face.mNumIndices; j++)
                        indices.push_back(face.mIndices[j]);
                }

                mesh->setGeometry(vertices.data(), vertices.size(), indices.data(), indices.size());
                mesh->updateBuffers();
                result.meshes.push_back(mesh);
            }
        }

        auto benchEnd = steady_clock::now();
        cout << filePath << " - load took " << duration<float>(benchEnd - benchStart).count() << " seconds" << endl;
        return result;
    }
}
