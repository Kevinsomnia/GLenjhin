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
uniform sampler2D u_ShaftsTex;
uniform vec4 u_Color;

in vec2 v_UV;

out vec4 fragColor;

void main()
{
    vec4 screen = texture2D(u_MainTex, v_UV);
    vec4 shafts = texture2D(u_ShaftsTex, v_UV);
    fragColor = vec4(screen.rgb + (shafts.rgb * u_Color.rgb), 1.0);
}
