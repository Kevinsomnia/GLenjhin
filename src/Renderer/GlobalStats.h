#pragma once

#include <iostream>
#include <vector>

#include "Texture/Texture.h"
#include "Shader.h"


// Forward declarations
class Shader;
class Texture;


class GlobalStats
{
public:
    static void Init();
    static void Reset();
    static uint32_t GetDrawCalls();
    static size_t GetVertexCount();
    static size_t GetIndexCount();
    static uint32_t GetShaderCalls();
    static uint32_t GetFramebufferBindCalls();
    static void AddDrawCall();
    static void AddVertexCount(size_t verts);
    static void AddIndexCount(size_t tris);
    static void AddShaderCall();
    static void AddFramebufferBindCall();

    static uint32_t GetMeshRendererCount();
    static std::vector<Shader*>& GetActiveShaders();
    static std::vector<Texture*>& GetActiveTextures();
    static void AddMeshRendererCount();
    static void AddActiveShader(Shader* shader);
    static void AddActiveTexture(Texture* texture);
    static void RemoveMeshRendererCount();
    static void RemoveActiveShader(Shader* shader);
    static void RemoveActiveTexture(Texture* texture);
private:
    // Frame rendering stats
    static uint32_t drawCalls;
    static size_t vertexCount;
    static size_t indexCount;
    static uint32_t shaderCalls;
    static uint32_t framebufferBindCalls;
    // Memory/objects
    static uint32_t meshRendererCount;
    static std::vector<Shader*> activeShaders;
    static std::vector<Texture*> activeTextures;
};
