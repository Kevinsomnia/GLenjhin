#pragma vertex
#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 u_VP;
uniform mat4 u_Model;

out vec3 v_Pos;
out vec3 v_Normal;
out vec2 v_UV;

void main()
{
    vec4 worldPos = u_Model * aPosition;

    v_Pos = worldPos.xyz;
    v_Normal = transpose(inverse(mat3(u_Model))) * aNormal;
    v_UV = vec2(aUV.x, 1.0 - aUV.y);

    gl_Position = u_VP * worldPos;
}



#pragma fragment
#version 330 core

uniform vec3 u_CameraPos;
uniform mat4 u_DirLightMatrix;
uniform vec3 u_DirLightDir;
uniform vec4 u_DirLightColor;

uniform sampler2D u_MainTex;
uniform vec4 u_EmissionColor;

// Shadowmaps
uniform sampler2D u_DirShadows;
uniform vec2 u_DirShadowsTexelSize;

in vec3 v_Pos;
in vec3 v_Normal;
in vec2 v_UV;

out vec4 fragColor;


const float EPSILON = 0.000001;
const float MIN_SHADOW_BIAS = 0.00003;
const float MAX_SHADOW_BIAS = 0.0006;


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
    float r = 1.0 + roughness;
    float k = (r * r) / 8.0;
    return d / (d * (1.0 - k) + k);
}

float GeometrySmith(float nDotL, float nDotV, float smoothness)
{
    float roughness = 1.0 - smoothness;
    return GeometrySchlickGGX(nDotL, roughness) * GeometrySchlickGGX(nDotV, roughness);
}


// Forward rendering
void main()
{
    vec4 albedo = texture(u_MainTex, v_UV);
    vec3 nrm = normalize(v_Normal);

    // Per-view calculations.
    // TODO: metallic and smoothness maps
    const float METALLIC = 0.1;
    const float SMOOTHNESS = 0.75;
    vec3 F0 = mix(vec3(0.04), albedo.rgb, METALLIC);     // Di-electric / Metallic: specular color
    vec3 viewDir = normalize(u_CameraPos - v_Pos);
    vec3 L = vec3(0.0);

    // Per-light calculations.
    vec3 halfDir = normalize(viewDir - u_DirLightDir);
    float nDotL = dot(-u_DirLightDir, nrm);

    if (nDotL > 0.0)
    {
        float nDotV = max(0.0, dot(nrm, viewDir));

        float distribution = DistributionGGX(nrm, halfDir, SMOOTHNESS);
        vec3 fresnel = FresnelSchlick(F0, dot(viewDir, halfDir));
        float geometry = GeometrySmith(nDotL, nDotV, SMOOTHNESS);

        // Compute Cook-Torrance BRDF for specular term: DFG / [4dot(wo, N)dot(wi, N)]
        // Where wo = view direction (solid angle), wi = light direction
        vec3 specular = (distribution * fresnel * geometry) / (4.0 * nDotV * nDotL + EPSILON);

        vec3 kDiff = vec3(1.0) - fresnel;
        kDiff *= 1.0 - METALLIC;

        vec4 dirLightFragPos = u_DirLightMatrix * vec4(v_Pos, 1.0);
        float shadow = ShadowAttenuation(u_DirShadows, u_DirShadowsTexelSize, dirLightFragPos, u_DirLightDir, nrm);
        vec3 brdf = (kDiff * ONE_OVER_PI * albedo.rgb) + specular;
        L += brdf * u_DirLightColor.rgb * nDotL * shadow;
    }

    vec3 ambient = vec3(0.196, 0.302, 0.349) * albedo.rgb;  // needs uniform
    fragColor = vec4(ambient + L + u_EmissionColor.rgb, 1.0);
}
