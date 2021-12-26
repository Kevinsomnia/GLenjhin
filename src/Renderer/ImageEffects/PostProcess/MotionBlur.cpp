#include "MotionBlur.h"

MotionBlur::MotionBlur() : PostProcessEffect("res\\shaders\\ImageEffects\\Common\\Copy.glsl")
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

    if (m_Initialized)
    {
        delete m_PackedVelocityDepthTex;

        for (size_t i = 0; i < m_MaxVelocityTex.size(); i++)
            delete m_MaxVelocityTex[i];

        delete m_MaxTileVelocityTex;
        delete m_MaxNeighborVelocityTex;
    }
}

void MotionBlur::lazyInitialize(Camera* camera)
{
    if (m_Initialized)
        return;

    Texture2D* motionVectorsTex = camera->getMotionVectorsTexture();

    if (!motionVectorsTex)
    {
        cerr << "No motion vectors found. Motion vectors are only supported in deferred rendering. Ignoring MotionBlur initialization." << endl;
        return;
    }

    PostProcessEffect::lazyInitialize(camera);
    BufferTexture* renderTarget = camera->getRenderTargetBuffer();
    uint32_t maxBlurRadius = static_cast<uint32_t>(renderTarget->height() * MAX_BLUR_RADIUS);   // in pixels

    uint32_t w = renderTarget->width();
    uint32_t h = renderTarget->height();

    m_PackedVelocityDepthTex = new BufferTexture(w, h, /*depth=*/ 0, TextureFormat::RGB10A2);
    m_PackedVelocityDepthTex->setFilterMode(TextureFilterMode::Point);

    for (size_t i = 0; i < m_MaxVelocityTex.size(); i++)
    {
        size_t downsample = i + 1;
        m_MaxVelocityTex[i] = new BufferTexture(w >> downsample, h >> downsample, /*depth=*/ 0, TextureFormat::RGHalf);
        m_MaxVelocityTex[i]->setFilterMode(TextureFilterMode::Point);
    }

    // The screen is divided into tiles (8x8 pixels), and those controls how big those tiles are in pixels, and therefore must be a multiple of 8.
    // This defines the region of pixels that blur can potentially affect if the sample is at the center of the tile.
    uint32_t tileSize = ((maxBlurRadius - 1) / 8 + 1) * 8;
    assert(tileSize != 0);
    m_MaxTileVelocityTex = new BufferTexture(w / tileSize, h / tileSize, /*depth=*/ 0, TextureFormat::RGHalf);
    m_MaxTileVelocityTex->setFilterMode(TextureFilterMode::Point);
    // The max 3x3 neighbor mega-tile pass will handle cases where we're attempting to sample near the edges of the mega-tile and the blur can spill to neighboring mega-tiles.
    m_MaxNeighborVelocityTex = new BufferTexture(w / tileSize, h / tileSize, /*depth=*/ 0, TextureFormat::RGHalf);
    m_MaxNeighborVelocityTex->setFilterMode(TextureFilterMode::Point);

    Vector2 motionVectorsTexSize = motionVectorsTex->size();
    m_PackVelocityDepthMat->setTexture("u_MotionTex", motionVectorsTex);
    m_PackVelocityDepthMat->setTexture("u_DepthTex", m_Camera->getDepthTexture());
    m_PackVelocityDepthMat->setVector3("u_MotionParams", Vector3(motionVectorsTexSize.x, motionVectorsTexSize.y, static_cast<float>(maxBlurRadius)));
    m_PackVelocityDepthMat->setVector4("u_ProjectionParams", camera->getProjectionParams());
    m_PackVelocityDepthMat->setFloat("u_VelocityScale", BLUR_STRENGTH);

    m_MaxVelocityDenormalizeMat->setTexture("u_VelocityTex", m_PackedVelocityDepthTex->colorTexture());
    m_MaxVelocityDenormalizeMat->setVector2("u_VelocityTexelSize", m_PackedVelocityDepthTex->texelSize());
    m_MaxVelocityDenormalizeMat->setFloat("u_MaxBlurRadius", static_cast<float>(maxBlurRadius));

    BufferTexture* max8x8 = m_MaxVelocityTex[m_MaxVelocityTex.size() - 1];
    m_MaxTileVelocityMat->setTexture("u_VelocityTex", max8x8->colorTexture());
    m_MaxTileVelocityMat->setVector2("u_VelocityTexelSize", max8x8->texelSize());
    float tileUvOffset = (tileSize / 8.0f - 1.0f) * -0.5f;  // offset to get bottom-left corner of mega-tile.
    m_MaxTileVelocityMat->setVector2("u_TileParams", Vector2(tileUvOffset, static_cast<float>(tileSize / 8))); // X = tile size, Y = tile iterations

    m_MaxNeighborVelocityMat->setTexture("u_VelocityTex", m_MaxTileVelocityTex->colorTexture());
    m_MaxNeighborVelocityMat->setVector2("u_VelocityTexelSize", m_MaxTileVelocityTex->texelSize());

    m_ReconstructionMat->setTexture("u_PackedTex", m_PackedVelocityDepthTex->colorTexture());
    m_ReconstructionMat->setVector2("u_PackedTexSize", m_PackedVelocityDepthTex->size());
    m_ReconstructionMat->setVector2("u_PackedTexelSize", m_PackedVelocityDepthTex->texelSize());
    m_ReconstructionMat->setTexture("u_MaxNeighborTex", m_MaxNeighborVelocityTex->colorTexture());
    m_ReconstructionMat->setVector2("u_MaxNeighborTexelSize", m_MaxNeighborVelocityTex->texelSize());
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

    // Pack velocity and depth into one texture, while clamping the range of velocity values.
    PostProcessEffect::render(m_PackedVelocityDepthTex, m_PackVelocityDepthMat);

    // Retrieve max pixel velocity for the tile (8x8 pixel block / 3-pass downsample).
    PostProcessEffect::render(m_MaxVelocityTex[0], m_MaxVelocityDenormalizeMat);

    for (size_t i = 1; i < m_MaxVelocityTex.size(); i++)
    {
        m_MaxVelocityMat->setTexture("u_VelocityTex", m_MaxVelocityTex[i - 1]->colorTexture());
        m_MaxVelocityMat->setVector2("u_VelocityTexelSize", m_MaxVelocityTex[i - 1]->texelSize());
        PostProcessEffect::render(m_MaxVelocityTex[i], m_MaxVelocityMat);
    }

    // Last downsample and retrieve max velocity of the mega-tile size (region of multiple tiles, or 8x8 pixel blocks).
    PostProcessEffect::render(m_MaxTileVelocityTex, m_MaxTileVelocityMat);

    // Get max velocity within a 3x3 mega-tile region (tile neighbors), but slight bias on center tile.
    PostProcessEffect::render(m_MaxNeighborVelocityTex, m_MaxNeighborVelocityMat);

    // Finally, apply motion blur
    m_ReconstructionMat->setFloat("u_Time", static_cast<float>(Time::GetTime()));
    PostProcessEffect::render(source, destination, m_ReconstructionMat);
}
