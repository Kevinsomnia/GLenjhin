#include "GlobalStats.h"

uint32_t GlobalStats::drawCalls = 0;
size_t GlobalStats::vertexCount = 0;
size_t GlobalStats::indexCount = 0;
uint32_t GlobalStats::shaderCalls = 0;
uint32_t GlobalStats::framebufferBindCalls = 0;

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
