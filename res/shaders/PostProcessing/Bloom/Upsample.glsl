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

uniform sampler2D u_PrevTex;    // Downsampled bloom
uniform sampler2D u_MainTex;
uniform vec2 u_BlurSize;

in vec2 v_UV;

out vec4 fragColor;

void main()
{
    vec4 color = texture2D(u_MainTex, v_UV);

    // Apply 9-tap tent filter on downsampled texture.
    vec4 delta = u_BlurSize.xyxy * vec4(1.0, 1.0, -1.0, 0.0);

    vec4 prev = texture2D(u_PrevTex, v_UV + delta.zy);
    prev += texture2D(u_PrevTex, v_UV + delta.wy) * 2.0;
    prev += texture2D(u_PrevTex, v_UV + delta.xy);

    prev += texture2D(u_PrevTex, v_UV + delta.zw) * 2.0;
    prev += texture2D(u_PrevTex, v_UV) * 4.0;
    prev += texture2D(u_PrevTex, v_UV + delta.xw) * 2.0;

    prev += texture2D(u_PrevTex, v_UV - delta.xy);
    prev += texture2D(u_PrevTex, v_UV - delta.wy) * 2.0;
    prev += texture2D(u_PrevTex, v_UV - delta.zy);

    prev *= 0.0625;

    fragColor = vec4(color.rgb + prev.rgb, 1.0);
}
