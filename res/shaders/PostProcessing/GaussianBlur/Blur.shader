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
uniform vec2 u_Stride;

in vec2 v_UV;

out vec4 fragColor;

void main()
{
    vec3 color = texture2D(u_MainTex, v_UV).rgb * 0.227027027;

    vec2 delta = u_Stride;
    color += texture2D(u_MainTex, v_UV - delta).rgb * 0.3162162162;
    color += texture2D(u_MainTex, v_UV + delta).rgb * 0.3162162162;

    delta = u_Stride * 2.33333333;
    color += texture2D(u_MainTex, v_UV - delta).rgb * 0.0702702703;
    color += texture2D(u_MainTex, v_UV + delta).rgb * 0.0702702703;

    fragColor = vec4(color, 1.0);
}
