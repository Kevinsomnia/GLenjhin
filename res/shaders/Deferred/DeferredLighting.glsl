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

uniform vec3 u_CameraPos;
uniform mat4 u_DirLightMatrix;
uniform vec3 u_DirLightDir;
uniform vec3 u_DirLightColor;

in vec2 v_UV;

out vec4 fragColor;

const float SHININESS = 128.0;
const float EPSILON = 0.000001;
const float MIN_SHADOW_BIAS = 0.00002;
const float MAX_SHADOW_BIAS = 0.0004;

float BilinearSampleShadowMap(sampler2D tex, vec2 texelSize, vec2 uv)
{
    vec4 delta = texelSize.xyxy * vec4(1.0, 1.0, -1.0, -1.0);
    float value = texture2D(tex, uv + delta.zy).r;
    return value;
}

float ShadowAttenuation(vec4 dirLightFragPos, vec3 worldLightDir, vec3 worldNormal)
{
    // Perspective divide to get light-space NDC coords.
    vec3 lightCoords = dirLightFragPos.xyz / dirLightFragPos.w;

    // Ignore past light's far clip
    if (lightCoords.z > 1.0)
        return 1.0;

    // Normalize from 0 to 1.
    lightCoords = (lightCoords * 0.5) + 0.5;

    // Sample light's depth map.
    float lightDepth = BilinearSampleShadowMap(u_DirShadows, u_DirShadowsTexelSize, lightCoords.xy);
    float fragDepth = lightCoords.z;
    // Test if the light's depth is behind this fragment's depth. If so, return 0.
    // Since depth buffer is point filtered, we need to account for imprecision by using a bias value.
    // "Steeper" angles get affected more by this imprecision so we want to add more bias in those cases.
    float bias = mix(MIN_SHADOW_BIAS, MAX_SHADOW_BIAS, clamp(1.0 - dot(-worldLightDir, worldNormal), 0.0, 1.0));
    return clamp((lightDepth - (fragDepth - bias)) / EPSILON, 0.0, 1.0);
}

void main()
{
    vec4 position = texture2D(u_Position, v_UV);
    vec4 normalsSmoothness = texture2D(u_NormalSmooth, v_UV);
    vec4 albedoMetallic = texture2D(u_AlbedoMetal, v_UV);
    vec4 emission = texture2D(u_Emission, v_UV);

    vec3 ambient = vec3(0.1);   // needs uniform
    float shadow = ShadowAttenuation(u_DirLightMatrix * position, u_DirLightDir, normalsSmoothness.xyz);
    float nDotL = max(0.0, dot(-u_DirLightDir, normalsSmoothness.xyz) * shadow);

    vec3 viewDir = normalize(u_CameraPos - position.xyz);
    vec3 halfDir = normalize(viewDir - u_DirLightDir);
    float specContrib = max(0.0, dot(halfDir, normalsSmoothness.xyz));
    float specular = pow(specContrib, SHININESS) * shadow;

    fragColor = vec4(albedoMetallic.rgb * mix(ambient, vec3(1.0), nDotL) + specular + emission.rgb, 1.0);
}
