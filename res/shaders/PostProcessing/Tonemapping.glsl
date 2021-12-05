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

const float EXPOSURE = 3.25;

void main()
{
    vec4 color = texture2D(u_MainTex, v_UV);
    color = 1.0 - exp2(color * -EXPOSURE);
    fragColor = vec4(color.rgb, 1.0);
}
