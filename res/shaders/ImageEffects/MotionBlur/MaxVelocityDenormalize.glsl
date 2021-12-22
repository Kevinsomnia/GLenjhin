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

uniform sampler2D u_VelocityTex;
uniform vec2 u_VelocityTexelSize;
uniform float u_MaxBlurRadius;

in vec2 v_UV;

out vec2 fragColor;

vec2 Max(vec2 a, vec2 b)
{
    return (dot(a, a) > dot(b, b)) ? a : b;
}

void main()
{
    // Denormalize velocity vectors, then select the longest vector in a 2x2 sample.
    vec4 delta = u_VelocityTexelSize.xyxy * vec4(1.0, 1.0, -1.0, -1.0);

    vec2 veloTL = texture2D(u_VelocityTex, v_UV + delta.zy).rg * 2.0 - 1.0;
    vec2 veloTR = texture2D(u_VelocityTex, v_UV + delta.xy).rg * 2.0 - 1.0;
    vec2 veloBL = texture2D(u_VelocityTex, v_UV + delta.zw).rg * 2.0 - 1.0;
    vec2 veloBR = texture2D(u_VelocityTex, v_UV + delta.xw).rg * 2.0 - 1.0;
    vec2 maxVelo = Max(Max(Max(veloTL, veloTR), veloBL), veloBR);

    // Scale to max blur radius to fully unpack the motion data.
    fragColor = maxVelo * u_MaxBlurRadius;
}
