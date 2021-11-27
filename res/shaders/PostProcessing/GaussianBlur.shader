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

in vec2 v_UV;

out vec4 fragColor;

const float EXPOSURE = 3.5;

void main()
{
    const float DDX = 1.0 / 1600.0;
    const float DDY = 1.0 / 900.0;

    vec3 color = texture2D(u_MainTex, v_UV + vec2(-DDX, DDY)).rgb;
    color += texture2D(u_MainTex, v_UV + vec2(DDX, DDY)).rgb;
    color += texture2D(u_MainTex, v_UV + vec2(-DDX, -DDY)).rgb;
    color += texture2D(u_MainTex, v_UV + vec2(DDX, -DDY)).rgb;
    color *= 0.25;
    fragColor = vec4(color, 1.0);
}
