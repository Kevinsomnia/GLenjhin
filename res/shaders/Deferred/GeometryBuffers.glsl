#pragma vertex
#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aUV;

uniform mat4 u_PrevVP;
uniform mat4 u_CurrVP;
uniform mat4 u_PrevModel;
uniform mat4 u_CurrModel;

out vec3 v_Position;
out vec3 v_Normal;
out vec3 v_Tangent;
out vec2 v_UV;
out vec4 v_PrevPosNDC;
out vec4 v_CurrPosNDC;

void main()
{
    vec4 prevWorldPos = u_PrevModel * aPosition;
    vec4 currWorldPos = u_CurrModel * aPosition;
    mat3 normalMatrix = transpose(inverse(mat3(u_CurrModel)));

    v_Position = currWorldPos.xyz;
    v_Normal = normalMatrix * aNormal;
    v_Tangent = normalMatrix * aTangent;
    v_UV = vec2(aUV.x, 1.0 - aUV.y);

    vec4 currPos = u_CurrVP * currWorldPos;

    v_PrevPosNDC = u_PrevVP * prevWorldPos;
    v_CurrPosNDC = currPos;

    // NDC is scaled from [-1, 1], so we need to halve the values to normalize the delta to viewport scale [0, 1].
    v_PrevPosNDC.xy *= 0.5;
    v_CurrPosNDC.xy *= 0.5;

    gl_Position = currPos;
}



#pragma fragment
#version 330 core

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormalSmoothness;
layout(location = 2) out vec4 gAlbedoMetallic;
layout(location = 3) out vec4 gEmissionOcclusion;
layout(location = 4) out vec2 gMotionVectors;

in vec3 v_Position;
in vec3 v_Normal;
in vec3 v_Tangent;
in vec2 v_UV;
in vec4 v_PrevPosNDC;
in vec4 v_CurrPosNDC;

uniform sampler2D u_AlbedoTex;
uniform sampler2D u_NormalTex;
uniform sampler2D u_MSATex;
uniform sampler2D u_HeightTex;

uniform float u_HeightScale;
uniform vec2 u_HeightTexelSize;
uniform vec3 u_CameraPos;
uniform vec2 u_TileSize;
uniform vec4 u_EmissionColor;

// POM constants.
const float MIN_STEPS = 4.0;
const float MAX_STEPS = MIN_STEPS * 4.0;

vec3 CalculateParallaxTangentNormal(vec2 uv)
{
    vec3 delta = u_HeightTexelSize.xyx * vec3(1.0, 1.0, 0.0);
    float heightN = texture2D(u_HeightTex, uv + delta.zy).r;
    float heightS = texture2D(u_HeightTex, uv - delta.zy).r;
    float heightE = texture2D(u_HeightTex, uv + delta.xz).r;
    float heightW = texture2D(u_HeightTex, uv - delta.xz).r;

    // Approximate normal by calculating the slope from the 4 height samples.
    float deltaX = (heightW - heightE) / u_HeightTexelSize.x * u_HeightScale;
    float deltaY = (heightS - heightN) / u_HeightTexelSize.y * u_HeightScale;
    return normalize(vec3(deltaX, deltaY, 1.0));
}

vec2 ParallaxUV(vec2 uv, mat3 worldTBN, vec3 worldViewDir)
{
    // Parallax works in tangent-space, so we have to convert the world space view direction before feeding to parallax.
    // Transpose of orthonormal matrices has the same effect as taking the inverse, but it's faster.
    vec3 tangentViewDir = transpose(worldTBN) * worldViewDir;

    // If tangentViewDir is (0, 0, 1), then we are looking straight down on the surface, so use minimum amount of depth steps for optimization.
    float steps = mix(MIN_STEPS, MAX_STEPS, 1.0 - tangentViewDir.z);

    // Sampling interval, taking into account perspective distortion (closer to surface = distort more).
    vec2 sampleStride = -((tangentViewDir.xy / tangentViewDir.z) * u_HeightScale) / steps;
    float stepDepth = 1.0 / steps;
    vec2 sampleCoord = uv;
    vec2 prevSampleCoord = sampleCoord;

    // Instead of protruding the depth outwards along the normal, we invert it to avoid depth test artifacts with other meshes as much as possible.
    float sampleHeightDepth = 1.0 - texture2D(u_HeightTex, sampleCoord).r;
    float prevSampleHeightDepth = sampleHeightDepth;
    float currDepth = 0.0;

    while (currDepth < sampleHeightDepth)
    {
        prevSampleCoord = sampleCoord;
        sampleCoord += sampleStride;
        prevSampleHeightDepth = sampleHeightDepth;
        sampleHeightDepth = 1.0 - texture2D(u_HeightTex, sampleCoord).r;
        currDepth += stepDepth;
    }

    float blend = clamp(1.0 - ((currDepth - sampleHeightDepth) / stepDepth), 0.0, 1.0);
    return mix(prevSampleCoord, sampleCoord, blend);
}

void main()
{
    // Create the world-space TBN for normal-map and parallax calculations. Use Gram-Schmidt process to reorthogonalize.
    vec3 tangent = normalize(v_Tangent);
    vec3 normal = normalize(v_Normal);
    tangent = normalize(tangent - (normal * dot(normal, tangent)));
    vec3 bitangent = cross(normal, tangent);
    mat3 worldTBN = mat3(tangent, bitangent, normal);

    vec2 uv = v_UV;
    vec3 heightTangentNormal = vec3(0.0, 0.0, 1.0);

    if (u_HeightScale > 0.0)
    {
        // Apply POM (manipulates/distorts UV).
        vec3 worldViewDir = normalize(u_CameraPos - v_Position);
        uv = ParallaxUV(uv, worldTBN, worldViewDir);

        // Outside of sampling range, so we discard pixel which will allows us to see through the surface where the depth check doesn't hit.
        // Pretty convincing on quads near their edges, but may fail on more complex surface types.
        if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
            discard;

        // Compute tangent space normal from parallax map.
        heightTangentNormal = CalculateParallaxTangentNormal(uv);
    }

    uv *= u_TileSize;
    vec3 albedo = texture2D(u_AlbedoTex, uv).rgb;
    vec3 nrm = texture2D(u_NormalTex, uv).rgb;
    vec3 msa = texture2D(u_MSATex, uv).rgb;

    // Construct the final world-space normal by applying the tangent-space normal vector onto the world TBN.
    vec3 tangentOffset = normalize((nrm * 2.0) - 1.0);
    // Add normal contribution from parallax map. This is an approximation, but the ideal solution would be to create a TBN for `heightTangentNormal`.
    tangentOffset = normalize(tangentOffset + heightTangentNormal);
    vec3 finalWorldNormal = normalize(worldTBN * tangentOffset);

    gPosition = vec4(v_Position, 1.0);
    gNormalSmoothness = vec4(finalWorldNormal, msa.g);
    gAlbedoMetallic = vec4(albedo, msa.r);
    gEmissionOcclusion = vec4(u_EmissionColor.rgb, msa.b);

    // Compute motion vectors in viewport space [0, 1].
    vec2 prevViewportPos = v_PrevPosNDC.xy / v_PrevPosNDC.w;
    vec2 currViewportPos = v_CurrPosNDC.xy / v_CurrPosNDC.w;
    gMotionVectors = currViewportPos - prevViewportPos;
}
