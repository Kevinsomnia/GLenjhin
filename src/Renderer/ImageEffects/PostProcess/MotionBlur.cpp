#include "MotionBlur.h"

MotionBlur::MotionBlur() : PostProcessEffect("res\\shaders\\ImageEffects\\Common\\Copy.glsl"), m_TileSize(0)
{
    m_PackVelocityDepthMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\MotionBlur\\PackVelocityDepth.glsl"));
    m_MaxVelocityDenormalizeMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\MotionBlur\\MaxVelocityDenormalize.glsl"));
    m_MaxVelocityMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\MotionBlur\\MaxVelocity.glsl"));
    m_MaxTileVelocityMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\MotionBlur\\MaxTileVelocity.glsl"));
    m_MaxNeighborVelocityMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\MotionBlur\\MaxNeighborVelocity.glsl"));
    m_ReconstructionMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\MotionBlur\\Reconstruction.glsl"));
}

MotionBlur::~MotionBlur()
{
    delete m_PackVelocityDepthMat;
    delete m_MaxVelocityDenormalizeMat;
    delete m_MaxVelocityMat;
    delete m_MaxTileVelocityMat;
    delete m_MaxNeighborVelocityMat;
    delete m_ReconstructionMat;
}

void MotionBlur::lazyInitialize(Camera* camera)
{
    if (m_Initialized)
        return;

    Texture2D* motionVectorsTex = camera->motionVectorsTexture();

    if (!motionVectorsTex)
    {
        cerr << "No motion vectors found. Motion vectors are only supported in deferred rendering. Ignoring MotionBlur initialization." << endl;
        return;
    }

    PostProcessEffect::lazyInitialize(camera);

    uint32_t maxBlurRadius = static_cast<uint32_t>(camera->renderTargetBuffer()->height() * MAX_BLUR_RADIUS);   // in pixels

    // The screen is divided into tiles (8x8 pixels), and those controls how big those tiles are in pixels, and therefore must be a multiple of 8.
    // This defines the region of pixels that blur can potentially affect if the sample is at the center of the tile.
    m_TileSize = ((maxBlurRadius - 1) / 8 + 1) * 8;
    assert(m_TileSize != 0);

    Vector2 motionVectorsTexSize = motionVectorsTex->size();
    m_PackVelocityDepthMat->setTexture("u_MotionTex", motionVectorsTex);
    m_PackVelocityDepthMat->setTexture("u_DepthTex", m_Camera->depthTexture());
    m_PackVelocityDepthMat->setVector3("u_MotionParams", Vector3(motionVectorsTexSize.x, motionVectorsTexSize.y, static_cast<float>(maxBlurRadius)));
    m_PackVelocityDepthMat->setVector4("u_ProjectionParams", camera->projectionParams());
    m_PackVelocityDepthMat->setFloat("u_VelocityScale", BLUR_STRENGTH);

    m_MaxVelocityDenormalizeMat->setFloat("u_MaxBlurRadius", static_cast<float>(maxBlurRadius));

    float tileUvOffset = (m_TileSize / 8.0f - 1.0f) * -0.5f;  // offset to get bottom-left corner of mega-tile.
    m_MaxTileVelocityMat->setVector2("u_TileParams", Vector2(tileUvOffset, static_cast<float>(m_TileSize / 8))); // X = tile size, Y = tile iterations

    m_ReconstructionMat->setFloat("u_MaxBlurRadius", static_cast<float>(maxBlurRadius));
    m_ReconstructionMat->setFloat("u_SampleCount", static_cast<float>(SAMPLE_COUNT));
}

void MotionBlur::render(BufferTexture* source, BufferTexture* destination)
{
    if (!m_Initialized)
    {
        PostProcessEffect::render(source, destination);
        return;
    }

    // Attempt to get consistent blurring results across various frame-rates.
    float deltaTimeAdjustment = 0.01f / static_cast<float>(Math::Max(0.001, Time::GetDeltaTime()));
    m_PackVelocityDepthMat->setFloat("u_VelocityScale", BLUR_STRENGTH * deltaTimeAdjustment);

    uint32_t w = source->width();
    uint32_t h = source->height();

    // Pack velocity and depth into one texture, while clamping the range of velocity values.
    BufferTexture* packedVelocityDepthTex = BufferTexturePool::Get(w, h, /*colorFormat=*/ TextureFormat::RGB10A2, /*depthFormat=*/ TextureFormat::None, TextureFilterMode::Point);
    PostProcessEffect::render(packedVelocityDepthTex, m_PackVelocityDepthMat);

    for (size_t i = 0; i < m_MaxVelocityTex.size(); i++)
    {
        size_t downsample = i + 1;
        m_MaxVelocityTex[i] = BufferTexturePool::Get(w >> downsample, h >> downsample, /*colorFormat=*/ TextureFormat::RGHalf, /*depthFormat=*/ TextureFormat::None, TextureFilterMode::Point);
    }

    // Retrieve max pixel velocity for the tile (8x8 pixel block / 3-pass downsample).
    m_MaxVelocityDenormalizeMat->setTexture("u_VelocityTex", packedVelocityDepthTex->colorTexture());
    m_MaxVelocityDenormalizeMat->setVector2("u_VelocityTexelSize", packedVelocityDepthTex->texelSize());
    PostProcessEffect::render(m_MaxVelocityTex[0], m_MaxVelocityDenormalizeMat);

    for (size_t i = 1; i < m_MaxVelocityTex.size(); i++)
    {
        m_MaxVelocityMat->setTexture("u_VelocityTex", m_MaxVelocityTex[i - 1]->colorTexture());
        m_MaxVelocityMat->setVector2("u_VelocityTexelSize", m_MaxVelocityTex[i - 1]->texelSize());
        PostProcessEffect::render(m_MaxVelocityTex[i], m_MaxVelocityMat);
        BufferTexturePool::Return(m_MaxVelocityTex[i - 1]);
    }

    // Last downsample and retrieve max velocity of the mega-tile size (region of multiple tiles, or 8x8 pixel blocks).
    BufferTexture* max8x8Tex = m_MaxVelocityTex[m_MaxVelocityTex.size() - 1];
    BufferTexture* maxTileVeloTex = BufferTexturePool::Get(w / m_TileSize, h / m_TileSize, /*colorFormat=*/ TextureFormat::RGHalf, /*depthFormat=*/ TextureFormat::None, TextureFilterMode::Point);
    m_MaxTileVelocityMat->setTexture("u_VelocityTex", max8x8Tex->colorTexture());
    m_MaxTileVelocityMat->setVector2("u_VelocityTexelSize", max8x8Tex->texelSize());
    PostProcessEffect::render(maxTileVeloTex, m_MaxTileVelocityMat);
    BufferTexturePool::Return(max8x8Tex);

    // Get max velocity within a 3x3 mega-tile region (tile neighbors), but slight bias on center tile.
    // The max 3x3 neighbor mega-tile pass will handle cases where we're attempting to sample near the edges of the mega-tile and the blur can spill to neighboring mega-tiles.
    BufferTexture* maxNeighborVeloTex = BufferTexturePool::Get(w / m_TileSize, h / m_TileSize, /*colorFormat=*/ TextureFormat::RGHalf, /*depthFormat=*/ TextureFormat::None, TextureFilterMode::Point);
    m_MaxNeighborVelocityMat->setTexture("u_VelocityTex", maxTileVeloTex->colorTexture());
    m_MaxNeighborVelocityMat->setVector2("u_VelocityTexelSize", maxTileVeloTex->texelSize());
    PostProcessEffect::render(maxNeighborVeloTex, m_MaxNeighborVelocityMat);
    BufferTexturePool::Return(maxTileVeloTex);

    // Finally, apply motion blur
    m_ReconstructionMat->setFloat("u_Time", static_cast<float>(Time::GetTime()));
    m_ReconstructionMat->setTexture("u_PackedTex", packedVelocityDepthTex->colorTexture());
    m_ReconstructionMat->setVector2("u_PackedTexSize", packedVelocityDepthTex->size());
    m_ReconstructionMat->setVector2("u_PackedTexelSize", packedVelocityDepthTex->texelSize());
    m_ReconstructionMat->setTexture("u_MaxNeighborTex", maxNeighborVeloTex->colorTexture());
    m_ReconstructionMat->setVector2("u_MaxNeighborTexelSize", maxNeighborVeloTex->texelSize());
    PostProcessEffect::render(source, destination, m_ReconstructionMat);

    BufferTexturePool::Return(packedVelocityDepthTex);
    BufferTexturePool::Return(maxNeighborVeloTex);
}
