#include "GlobalStats.h"

uint32_t GlobalStats::drawCalls = 0;
size_t GlobalStats::vertexCount = 0;
size_t GlobalStats::indexCount = 0;
uint32_t GlobalStats::shaderCalls = 0;
uint32_t GlobalStats::framebufferBindCalls = 0;
uint32_t GlobalStats::meshRendererCount;
std::vector<Shader*> GlobalStats::activeShaders;
std::vector<Texture*> GlobalStats::activeTextures;

void GlobalStats::Init()
{
    Reset();
    meshRendererCount = 0;
    activeShaders.clear();
    activeTextures.clear();
}

void GlobalStats::Reset()
{
    drawCalls = 0;
    vertexCount = 0;
    indexCount = 0;
    shaderCalls = 0;
    framebufferBindCalls = 0;
}

uint32_t GlobalStats::GetDrawCalls()
{
    return drawCalls;
}

size_t GlobalStats::GetVertexCount()
{
    return vertexCount;
}

size_t GlobalStats::GetIndexCount()
{
    return indexCount;
}

uint32_t GlobalStats::GetShaderCalls()
{
    return shaderCalls;
}

uint32_t GlobalStats::GetFramebufferBindCalls()
{
    return framebufferBindCalls;
}



void GlobalStats::AddDrawCall()
{
    drawCalls++;
}

void GlobalStats::AddVertexCount(size_t verts)
{
    vertexCount += verts;
}

void GlobalStats::AddIndexCount(size_t tris)
{
    indexCount += tris;
}

void GlobalStats::AddShaderCall()
{
    shaderCalls++;
}

void GlobalStats::AddFramebufferBindCall()
{
    framebufferBindCalls++;
}

uint32_t GlobalStats::GetMeshRendererCount()
{
    return meshRendererCount;
}

std::vector<Shader*>& GlobalStats::GetActiveShaders()
{
    return activeShaders;
}

std::vector<Texture*>& GlobalStats::GetActiveTextures()
{
    return activeTextures;
}

void GlobalStats::AddMeshRendererCount()
{
    meshRendererCount++;
}

void GlobalStats::AddActiveShader(Shader* shader)
{
    activeShaders.push_back(shader);
}

void GlobalStats::AddActiveTexture(Texture* texture)
{
    activeTextures.push_back(texture);
}

void GlobalStats::RemoveMeshRendererCount()
{
    meshRendererCount--;
}

void GlobalStats::RemoveActiveShader(Shader* shader)
{
    auto iter = std::find(activeShaders.begin(), activeShaders.end(), shader);

    if (iter != activeShaders.end())
        activeShaders.erase(iter);
}

void GlobalStats::RemoveActiveTexture(Texture* texture)
{
    auto iter = std::find(activeTextures.begin(), activeTextures.end(), texture);

    if (iter != activeTextures.end())
        activeTextures.erase(iter);
}
