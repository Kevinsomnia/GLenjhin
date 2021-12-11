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

void main()
{
    vec4 delta = u_TexelSize.xyxy * vec4(1.0, 1.0, -1.0, -1.0);

    vec3 color = texture2D(u_MainTex, v_UV + delta.zy).rgb;
    color += texture2D(u_MainTex, v_UV + delta.xy).rgb;
    color += texture2D(u_MainTex, v_UV + delta.zw).rgb;
    color += texture2D(u_MainTex, v_UV + delta.xw).rgb;
    color *= 0.25;

    fragColor = vec4(color, 1.0);
}
