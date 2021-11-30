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
uniform sampler2D u_ScreenTex;
uniform vec2 u_BlurSize;

in vec2 v_UV;

out vec4 fragColor;

const float INTENSITY = 0.85;

void main()
{
    vec4 screen = texture2D(u_ScreenTex, v_UV);

    // Apply 9-tap tent filter on bloom texture (half res at this point).
    vec4 delta = u_BlurSize.xyxy * vec4(1.0, 1.0, -1.0, 0.0);

    vec4 bloom = texture2D(u_MainTex, v_UV + delta.zy);
    bloom += texture2D(u_MainTex, v_UV + delta.wy) * 2.0;
    bloom += texture2D(u_MainTex, v_UV + delta.xy);

    bloom += texture2D(u_MainTex, v_UV + delta.zw) * 2.0;
    bloom += texture2D(u_MainTex, v_UV) * 4.0;
    bloom += texture2D(u_MainTex, v_UV + delta.xw) * 2.0;

    bloom += texture2D(u_MainTex, v_UV - delta.xy);
    bloom += texture2D(u_MainTex, v_UV - delta.wy) * 2.0;
    bloom += texture2D(u_MainTex, v_UV - delta.zy);

    bloom *= 0.0625;
    bloom *= INTENSITY;

    fragColor = vec4(screen.rgb + bloom.rgb, 1.0);
}
