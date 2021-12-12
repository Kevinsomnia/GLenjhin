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
uniform vec3 u_SunPos;
uniform float u_BlurDist;

in vec2 v_UV;

out vec4 fragColor;

#define SAMPLE_COUNT 6

void main()
{
    vec4 col = vec4(0.0);
    vec2 uv = v_UV;
    vec2 blurVec = (u_SunPos.xy - uv) * u_BlurDist;

    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        col += texture2D(u_MainTex, uv);
        uv += blurVec;
    }

    fragColor = col / SAMPLE_COUNT;
}
