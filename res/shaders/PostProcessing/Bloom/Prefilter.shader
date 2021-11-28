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

in vec2 v_UV;

out vec4 fragColor;

const float THRESHOLD = 1.0;
const float PFC = 0.30102999566;    // pre-calculated: -log10(EXPOSURE); EXPOSURE = 0.5
const float CUTOFF = THRESHOLD + (PFC * 10.0);

float Brightness(vec3 col)
{
    return col.r * 0.2126 + col.g * 0.7152 + col.b * 0.0722;
}

vec3 Median(vec3 a, vec3 b, vec3 c)
{
    return a + b + c - min(min(a, b), c) - max(max(a, b), c);
}

void main()
{
    vec3 delta = u_TexelSize.xyx * vec3(1.0, 1.0, 0.0);

    // 5-tap sample, choose median pixel color
    vec3 s0 = texture2D(u_MainTex, v_UV).rgb;
    vec3 s1 = texture2D(u_MainTex, v_UV - delta.xz).rgb;
    vec3 s2 = texture2D(u_MainTex, v_UV + delta.xz).rgb;
    vec3 s3 = texture2D(u_MainTex, v_UV - delta.zy).rgb;
    vec3 s4 = texture2D(u_MainTex, v_UV + delta.zy).rgb;

    s0 = Median(Median(s0, s1, s2), s3, s4);
    s0 *= clamp((Brightness(s0) - THRESHOLD) / CUTOFF, 0.0, 1.0);
    fragColor = vec4(s0, 1.0);
}
