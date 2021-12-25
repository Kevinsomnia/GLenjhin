#pragma once

#include <iostream>

class GlobalStats
{
public:
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
private:
    static uint32_t drawCalls;
    static size_t vertexCount;
    static size_t indexCount;
    static uint32_t shaderCalls;
    static uint32_t framebufferBindCalls;
};
