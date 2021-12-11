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
    vec4 delta2 = u_TexelSize.xyxy * vec4(2.0, 2.0, -2.0, 0.0);

    // 13-tap downsample
    // 4-tap box filter.
    vec4 color = texture2D(u_MainTex, v_UV + delta.zy);
    color += texture2D(u_MainTex, v_UV + delta.xy);
    color += texture2D(u_MainTex, v_UV + delta.zw);
    color += texture2D(u_MainTex, v_UV + delta.xw);

    // 9-tap tent filter.
    color += texture2D(u_MainTex, v_UV + delta2.zy);
    color += texture2D(u_MainTex, v_UV + delta2.wy) * 2.0;
    color += texture2D(u_MainTex, v_UV + delta2.xy);

    color += texture2D(u_MainTex, v_UV + delta2.zw) * 2.0;
    color += texture2D(u_MainTex, v_UV) * 4.0;
    color += texture2D(u_MainTex, v_UV + delta2.xw) * 2.0;

    color += texture2D(u_MainTex, v_UV - delta2.xy);
    color += texture2D(u_MainTex, v_UV - delta2.wy) * 2.0;
    color += texture2D(u_MainTex, v_UV - delta2.zy);

    color *= 0.05;
    fragColor = vec4(color.rgb, 1.0);
}
