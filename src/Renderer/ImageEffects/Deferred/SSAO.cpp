#include "SSAO.h"

SSAO::SSAO() : DeferredEffect("res\\shaders\\ImageEffects\\SSAO\\Apply.glsl")
{
    m_OcclusionMat = new Material(new Shader("res\\shaders\\ImageEffects\\SSAO\\Occlusion.glsl"));
    m_BlurMat = new Material(new Shader("res\\shaders\\ImageEffects\\GaussianBlur\\Blur.glsl"));
}

SSAO::~SSAO()
{
    delete m_OcclusionMat;
    delete m_BlurMat;

    if (m_Initialized)
    {
        delete m_EmissionOcclGBufferCopy;
        delete m_NoiseTex;

        for (size_t i = 0; i < m_OcclusionBuffers.size(); i++)
            delete m_OcclusionBuffers[i];
    }
}

void SSAO::lazyInitialize(Camera* camera)
{
    if (m_Initialized)
        return;

    DeferredEffect::lazyInitialize(camera);

    // Create a GBuffer copy of EmissionOcclusion to read from, since we can't read and write to it simulataneously.
    GeometryBuffers* gBufs = camera->getGBuffers();
    Texture2D* emissionOccl = gBufs->emissionOcclGBuffer();
    m_EmissionOcclGBufferCopy = new BufferTexture(/*width=*/ emissionOccl->width(), /*height=*/ emissionOccl->height(), /*depth=*/ 0, /*colorFormat=*/ emissionOccl->format());
    m_CopyMat->setTexture("u_MainTex", emissionOccl);

    // Occlusion buffers setup
    m_NoiseTex = new Texture2D("res\\textures\\dither_noise.png", /*generateMipmaps=*/ false, /*readable=*/ false);

    BufferTexture* cameraTargetTex = camera->getRenderTargetBuffer();
    uint32_t w = cameraTargetTex->width() >> DOWNSAMPLE;
    uint32_t h = cameraTargetTex->height() >> DOWNSAMPLE;

    for (size_t i = 0; i < m_OcclusionBuffers.size(); i++)
        m_OcclusionBuffers[i] = new BufferTexture(w, h, /*depth=*/ 0, TextureFormat::R8);

    m_Material->setTexture("u_EmissionOccl", m_EmissionOcclGBufferCopy->colorTexture());
    m_Material->setTexture("u_Occlusion", m_OcclusionBuffers[0]->colorTexture());

    m_OcclusionMat->setTexture("u_Position", gBufs->positionGBuffer());
    m_OcclusionMat->setTexture("u_NormalSmooth", gBufs->normalSmoothGBuffer());
    m_OcclusionMat->setTexture("u_Depth", gBufs->depthTexture());
    m_OcclusionMat->setTexture("u_Noise", m_NoiseTex);
    m_OcclusionMat->setVector2("u_NoiseTexelSize", m_NoiseTex->texelSize());
    m_OcclusionMat->setVector2("u_TexelSize", m_OcclusionBuffers[0]->texelSize());
    m_OcclusionMat->setFloat("u_Radius", 0.375f);
    m_OcclusionMat->setFloat("u_Intensity", 1.0f);
}

void SSAO::render()
{
    DeferredEffect::render();

    // Render to occlusion buffers.
    BufferTexture* firstBuf = m_OcclusionBuffers[0];
    BufferTexture* secondBuf = m_OcclusionBuffers[1];

    m_OcclusionMat->setMatrix("u_V", m_Camera->getViewMatrix());
    m_OcclusionMat->setMatrix("u_P", m_Camera->getProjectionMatrix());
    DeferredEffect::render(firstBuf, m_OcclusionMat);

    Vector2 occlusionBufferTexelSize = firstBuf->texelSize();
    Vector2 strideHorizontal = Vector2(occlusionBufferTexelSize.getX(), 0.0f);
    Vector2 strideVertical = Vector2(0.0f, occlusionBufferTexelSize.getY());

    for (int i = 0; i < BLUR_ITERATIONS; i++)
    {
        m_BlurMat->setVector2("u_Stride", strideHorizontal);
        DeferredEffect::render(firstBuf, secondBuf, m_BlurMat);
        m_BlurMat->setVector2("u_Stride", strideVertical);
        DeferredEffect::render(secondBuf, firstBuf, m_BlurMat);
    }

    // Read emission GBuffer to buffer copy.
    m_EmissionOcclGBufferCopy->bind();
    m_Triangle->setMaterial(m_CopyMat);
    m_Triangle->draw();

    // Write occlusion buffer to emission GBuffer alpha channel.
    m_Camera->getGBuffers()->bind();
    m_Triangle->setMaterial(m_Material);
    m_Triangle->draw();
}
