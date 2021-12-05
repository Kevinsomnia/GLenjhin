#pragma vertex
#version 330 core

layout(location = 0) in vec2 aPosition;

out vec2 v_UV;

void main()
{
    gl_Position = vec4(aPosition, 0.0, 1.0);
    v_UV = vec2(aPosition.x + 1.0, aPosition.y + 1.0) * 0.5;
}



#pragma fragment
#version 330 core

// GBuffers
uniform sampler2D u_Position;
uniform sampler2D u_NormalSmooth;
uniform sampler2D u_AlbedoMetal;
uniform sampler2D u_Emission;

// Shadowmaps
uniform sampler2D u_DirShadows;
uniform vec2 u_DirShadowsTexelSize;

uniform vec4 u_AmbientColor;
uniform vec3 u_CameraPos;
uniform mat4 u_DirLightMatrix;
uniform vec3 u_DirLightDir;
uniform vec4 u_DirLightColor;

in vec2 v_UV;

out vec4 fragColor;

const float EPSILON = 0.000001;
const float MIN_SHADOW_BIAS = 0.00002;
const float MAX_SHADOW_BIAS = 0.0004;

// ===================
// SHADOWS
// ===================

float SampleShadowMap(sampler2D tex, vec3 uv, vec3 worldLightDir, vec3 worldNormal)
{
    float lightDepth = texture2D(tex, uv.xy).r;
    float fragDepth = uv.z;
    // Test if the light's depth is behind this fragment's depth. If so, return 0.
    // Since depth buffer is point filtered, we need to account for imprecision by using a bias value.
    // "Steeper" angles get affected more by this imprecision so we want to add more bias in those cases.
    float bias = mix(MIN_SHADOW_BIAS, MAX_SHADOW_BIAS, clamp(1.0 - dot(-worldLightDir, worldNormal), 0.0, 1.0));
    return clamp((lightDepth - (fragDepth - bias)) / EPSILON, 0.0, 1.0);
}

float BilinearSampleShadowMap(sampler2D tex, vec2 texelSize, vec3 uv, vec3 worldLightDir, vec3 worldNormal)
{
    vec2 samplePoint = (uv.xy / texelSize) + vec2(0.5);
    vec2 samplePointFloor = floor(samplePoint);
    vec2 samplePointFract = samplePoint - samplePointFloor;
    samplePoint = samplePointFloor * texelSize;

    vec3 delta = texelSize.xyx * vec3(1.0, 1.0, 0.0);

    float bottomLeft = SampleShadowMap(tex, vec3(samplePoint, uv.z), worldLightDir, worldNormal);
    float bottomRight = SampleShadowMap(tex, vec3(samplePoint + delta.xz, uv.z), worldLightDir, worldNormal);
    float bottom = mix(bottomLeft, bottomRight, samplePointFract.x);

    float topLeft = SampleShadowMap(tex, vec3(samplePoint + delta.zy, uv.z), worldLightDir, worldNormal);
    float topRight = SampleShadowMap(tex, vec3(samplePoint + delta.xy, uv.z), worldLightDir, worldNormal);
    float top = mix(topLeft, topRight, samplePointFract.x);

    return mix(bottom, top, samplePointFract.y);
}

#define PCF_SAMPLE_COUNT 16
vec2 pcfOffsetsJittered[PCF_SAMPLE_COUNT] = vec2[] (
    vec2(0.4455, 0.25109), vec2(0.40033, 0.98933), vec2(1.16506, 1.07249), vec2(0.13022, 1.97584),
    vec2(-0.74049, 0.46329), vec2(-0.87497, 0.88598), vec2(-1.26421, 0.97358), vec2(-1.72868, 0.92694),
    vec2(-0.41143, -0.28414), vec2(-0.62969, -0.90792), vec2(-1.51151, -0.36388), vec2(-1.24892, -1.20944),
    vec2(0.66761, -0.52171), vec2(1.0005, -0.22421), vec2(1.65966, -0.0889), vec2(1.04328, -1.49243)
);

float ShadowAttenuation(sampler2D tex, vec2 texelSize, vec4 dirLightFragPos, vec3 worldLightDir, vec3 worldNormal)
{
    // Perspective divide to get light-space NDC coords.
    vec3 lightCoords = dirLightFragPos.xyz / dirLightFragPos.w;

    // Ignore past light's far clip
    if (lightCoords.z > 1.0)
        return 1.0;

    // Normalize XYZ coordinates.
    lightCoords = (lightCoords * 0.5) + 0.5;

    // Apply PCF
    float result = 0.0;
    for (int i = 0; i < PCF_SAMPLE_COUNT; ++i)
        result += BilinearSampleShadowMap(tex, texelSize, lightCoords + vec3(pcfOffsetsJittered[i] * texelSize, 0.0), worldLightDir, worldNormal);
    return result / PCF_SAMPLE_COUNT;
}


// ===================
// PBR
// ===================

const float PI = 3.141592653589793;
const float ONE_OVER_PI = 1.0 / PI;

// Micro-facets and light distribution/probability function.
float DistributionGGX(vec3 normal, vec3 halfDir, float smoothness)
{
    float roughness = 1.0 - smoothness;
    float a2 = roughness;
    a2 *= a2;

    float nDotH2 = dot(normal, halfDir);
    nDotH2 *= nDotH2;

    float b2 = 1.0 + nDotH2 * (a2 - 1.0);
    b2 *= b2;
    return a2 / (PI * b2);
}

// Simplified formula to approximate light interaction at different viewing angles.
// F0 = light response at incidence direction N (0 angle)
// cosTheta = dot(V, H), where V is viewDir, L is lightDir, and H is V+L
vec3 FresnelSchlick(vec3 F, float cosTheta)
{
    return F + ((vec3(1.0) - F) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0));
}

// Micro-facet self-shadowing and light attenuation.
// This will darken at grazing angles with higher roughness values.
float GeometrySchlickGGX(float d, float roughness)
{
    float d2 = d * d + EPSILON;
    return 2.0 / (1.0 + sqrt(1.0 + roughness * roughness * ((1.0 - d2) / d2)));
}

float GeometrySmith(float nDotL, float nDotV, float smoothness)
{
    float roughness = 1.0 - smoothness;
    return GeometrySchlickGGX(nDotL, roughness) * GeometrySchlickGGX(nDotV, roughness);
}


void main()
{
    vec4 position = texture2D(u_Position, v_UV);
    vec4 normalsSmoothness = texture2D(u_NormalSmooth, v_UV);
    vec4 albedoMetallic = texture2D(u_AlbedoMetal, v_UV);
    vec4 emission = texture2D(u_Emission, v_UV);

    // Per-view calculations.
    vec3 F0 = mix(vec3(0.04), albedoMetallic.rgb, albedoMetallic.a);    // Di-electric / Metallic: specular color
    vec3 viewDir = normalize(u_CameraPos - position.xyz);
    vec3 L = vec3(0.0);

    // Per-light calculations.
    vec3 halfDir = normalize(viewDir - u_DirLightDir);
    float nDotL = max(0.0, dot(normalsSmoothness.xyz, -u_DirLightDir));

    if (nDotL > 0.0)
    {
        float nDotV = max(0.0, dot(normalsSmoothness.xyz, viewDir));

        float distribution = DistributionGGX(normalsSmoothness.xyz, halfDir, normalsSmoothness.a);
        vec3 fresnel = FresnelSchlick(F0, dot(viewDir, halfDir));
        float geometry = GeometrySmith(nDotL, nDotV, normalsSmoothness.a);

        // Compute Cook-Torrance BRDF for specular term: DFG / [4dot(wo, N)dot(wi, N)]
        // Where wo = view direction (solid angle), wi = light direction
        vec3 specular = (distribution * fresnel * geometry) / (4.0 * nDotV * nDotL + EPSILON);

        // Energy conservation
        vec3 kDiff = vec3(1.0) - fresnel;
        kDiff *= 1.0 - albedoMetallic.a;

        // Attenuate with shadowmap
        float shadow = ShadowAttenuation(u_DirShadows, u_DirShadowsTexelSize, u_DirLightMatrix * position, u_DirLightDir, normalsSmoothness.xyz);
        vec3 brdf = (kDiff * ONE_OVER_PI * albedoMetallic.rgb) + specular;
        L += brdf * u_DirLightColor.rgb * nDotL * shadow;
    }

    vec3 ambient = u_AmbientColor.rgb * albedoMetallic.rgb;  // TODO: skybox cubemap (irradiance)
    fragColor = vec4(ambient + L + emission.rgb, 1.0);
}
