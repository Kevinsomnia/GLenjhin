#pragma vertex
#version 330 core

layout(location = 0) in vec2 aPosition;

uniform mat4 u_VP;

out vec2 v_UV;
out vec3 v_RayDir;

void main()
{
    v_UV = vec2(aPosition.x + 1.0, 1.0 - aPosition.y) * 0.5;
    v_RayDir = inverse(mat3(u_VP)) * vec3(aPosition.xy, 1.0);
    gl_Position = vec4(aPosition, 1.0, 1.0);
}



#pragma fragment
#version 330 core

uniform sampler2D u_MainTex;

in vec2 v_UV;
in vec3 v_RayDir;

out vec4 fragColor;

const float ONE_OVER_PI = 1.0 / 3.141592653589793;

void main()
{
    // Interpolated ray direction won't always be normalized, even if it was already normalized in vertex shader.
    vec3 worldDir = normalize(v_RayDir);

    vec2 uv = vec2(
        atan(worldDir.x, worldDir.z) * 0.5 * ONE_OVER_PI,   // [0, 2pi] -> [0, 1]
        (-asin(worldDir.y) * ONE_OVER_PI) + 0.5
    );

    vec4 color = textureLod(u_MainTex, uv, 0);
    fragColor = vec4(color.rgb, 1.0);
}
