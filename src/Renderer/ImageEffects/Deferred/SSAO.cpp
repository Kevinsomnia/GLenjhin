#include "SSAO.h"

SSAO::SSAO() : DeferredEffect("res\\shaders\\ImageEffects\\SSAO\\Apply.glsl")
{
    m_OcclusionMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\SSAO\\Occlusion.glsl"));
    m_BlurMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\GaussianBlur\\Blur.glsl"));
}

SSAO::~SSAO()
{
    delete m_OcclusionMat;
    delete m_BlurMat;

    if (m_Initialized)
        delete m_NoiseTex;
}

void SSAO::lazyInitialize(Camera* camera)
{
    if (m_Initialized)
        return;

    DeferredEffect::lazyInitialize(camera);

    m_NoiseTex = new Texture2D("res\\textures\\dither_noise.png", /*generateMipmaps=*/ false, /*readable=*/ false, /*sRGB=*/ false);

    GeometryBuffers* gBufs = camera->gBuffers();
    m_OcclusionMat->setTexture("u_Position", gBufs->positionGBuffer());
    m_OcclusionMat->setTexture("u_NormalSmooth", gBufs->normalSmoothGBuffer());
    m_OcclusionMat->setTexture("u_Depth", gBufs->depthTexture());
    m_OcclusionMat->setTexture("u_Noise", m_NoiseTex);
    m_OcclusionMat->setVector2("u_NoiseTexelSize", m_NoiseTex->texelSize());
    m_OcclusionMat->setFloat("u_Radius", 0.375f);
    m_OcclusionMat->setFloat("u_Intensity", 1.0f);
}

void SSAO::render()
{
    DeferredEffect::render();

    Texture2D* emissionOccl = m_Camera->gBuffers()->emissionOcclGBuffer();

    // Assume screen dimensions are equal to the GBuffer dimensions.
    uint32_t w = emissionOccl->width() >> DOWNSAMPLE;
    uint32_t h = emissionOccl->height() >> DOWNSAMPLE;

    // Create two occlusion buffers for blur pass.
    BufferTexture* bt0 = BufferTexturePool::Get(w, h, /*colorFormat=*/ TextureFormat::R8, /*depthFormat=*/ TextureFormat::None);
    BufferTexture* bt1 = (BLUR_ITERATIONS > 0) ? BufferTexturePool::Get(w, h, /*colorFormat=*/ TextureFormat::R8, /*depthFormat=*/ TextureFormat::None) : nullptr;

    // Render to occlusion buffers.
    Vector2 occlusionBufferTexelSize = bt0->texelSize();
    m_OcclusionMat->setMatrix4x4("u_V", m_Camera->viewMatrix());
    m_OcclusionMat->setMatrix4x4("u_P", m_Camera->projectionMatrix());
    m_OcclusionMat->setVector2("u_TexelSize", occlusionBufferTexelSize);
    DeferredEffect::render(bt0, m_OcclusionMat);

    Vector2 strideHorizontal = Vector2(occlusionBufferTexelSize.x, 0.0f);
    Vector2 strideVertical = Vector2(0.0f, occlusionBufferTexelSize.y);

    for (int i = 0; i < BLUR_ITERATIONS; i++)
    {
        m_BlurMat->setVector2("u_Stride", strideHorizontal);
        DeferredEffect::render(bt0, bt1, m_BlurMat);
        m_BlurMat->setVector2("u_Stride", strideVertical);
        DeferredEffect::render(bt1, bt0, m_BlurMat);
    }

    // Create a GBuffer copy of EmissionOcclusion to read from in the next pass, since we can't read and write to it simulataneously.
    BufferTexture* emissionOcclCopy = BufferTexturePool::Get(emissionOccl->width(), emissionOccl->height(), /*colorFormat=*/ emissionOccl->format(), /*depthFormat=*/ TextureFormat::None);
    DeferredEffect::render(emissionOccl, emissionOcclCopy, m_CopyMat);

    // Write occlusion buffer to emissionOccl GBuffer alpha channel.
    m_Camera->gBuffers()->bind();
    m_Material->setTexture("u_EmissionOccl", emissionOcclCopy->colorTexture());
    m_Material->setTexture("u_Occlusion", bt0->colorTexture());
    FullscreenTriangle::Draw(*m_Material, /*depthTest=*/ false);

    BufferTexturePool::Return(emissionOcclCopy);
    BufferTexturePool::Return(bt0);

    if (BLUR_ITERATIONS > 0)
        BufferTexturePool::Return(bt1);
}
