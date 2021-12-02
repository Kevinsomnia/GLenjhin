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

uniform sampler2D u_MainTex;
uniform vec2 u_TexelSize;
uniform float u_Threshold;
uniform float u_SoftKnee;

in vec2 v_UV;

out vec4 fragColor;

const float EPSILON = 0.00001;

vec3 Median(vec3 a, vec3 b, vec3 c)
{
    return a + b + c - min(min(a, b), c) - max(max(a, b), c);
}

void main()
{
    vec3 delta = u_TexelSize.xyx * vec3(1.0, 1.0, 0.0);

    // 5-tap sample, choose median pixel color.
    vec3 s0 = texture2D(u_MainTex, v_UV).rgb;
    vec3 s1 = texture2D(u_MainTex, v_UV - delta.xz).rgb;
    vec3 s2 = texture2D(u_MainTex, v_UV + delta.xz).rgb;
    vec3 s3 = texture2D(u_MainTex, v_UV - delta.zy).rgb;
    vec3 s4 = texture2D(u_MainTex, v_UV + delta.zy).rgb;
    s0 = Median(Median(s0, s1, s2), s3, s4);

    // Quadratic thresholding to smooth out transition from being under-threshold vs. over-threshold.
    float bright = max(max(s0.r, s0.g), s0.b);
    float curve = clamp(bright - u_Threshold - u_SoftKnee, 0.0, u_SoftKnee * 2.0);
    curve = 0.25 / u_SoftKnee * curve * curve;

    s0 *= max(curve, bright - u_Threshold) / max(EPSILON, bright);

    // Limit very bright pixel values to avoid low precision artifacts.
    s0 = min(s0, vec3(32768.0));
    fragColor = vec4(s0, 1.0);
}
