#pragma vertex
#version 330 core

layout(location = 0) in vec2 aPosition;

uniform mat4 u_VP;

out vec2 v_UV;
out vec3 v_RayDir;

void main()
{
    gl_Position = vec4(aPosition, 0.0, 1.0);
    v_UV = vec2(aPosition.x + 1.0, aPosition.y + 1.0) * 0.5;
    v_RayDir = inverse(mat3(u_VP)) * vec3(aPosition.xy, 1.0);
}



#pragma fragment
#version 330 core

uniform sampler2D u_MainTex;
uniform sampler2D u_DepthTex;
uniform vec4 u_ProjectionParams;

in vec2 v_UV;
in vec3 v_RayDir;

out vec4 fragColor;

const float FOG_DENSITY = 0.0025;
const vec3 FOG_COLOR = vec3(0.8, 0.95, 1.0);

// Convert NDC depth to a linear value: 0 = near, 1 = far
float LinearDepth01(float depth)
{
    float ndcZ = depth * 2.0 - 1.0;
    // (2 * near) / (near + far - ndcZ * (far - near))
    return (2.0 * u_ProjectionParams.x) / (u_ProjectionParams.x + u_ProjectionParams.y - ndcZ * u_ProjectionParams.z);
}

void main()
{
    vec4 scene = texture2D(u_MainTex, v_UV);

    // Interpolated ray direction won't always be normalized, even if it was already normalized in vertex shader.
    vec3 worldDir = normalize(v_RayDir);
    float rawDepth = texture2D(u_DepthTex, v_UV).r;
    float linearDepth = LinearDepth01(rawDepth);
    float fogFactor = linearDepth * u_ProjectionParams.y * FOG_DENSITY;

    // Exponential squared falloff. Ignore skybox (1.0 depth)
    fogFactor = rawDepth < 1.0 ? exp2(-fogFactor * fogFactor) : 1.0;

    fogFactor = clamp(fogFactor, 0.0, 1.0);
    fragColor = vec4(mix(FOG_COLOR, scene.rgb, fogFactor), 1.0);
}
