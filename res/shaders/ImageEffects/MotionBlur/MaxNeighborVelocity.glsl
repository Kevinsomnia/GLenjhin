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

in vec2 v_UV;

out vec2 fragColor;

// Slight bias on center mega-tile: if there are multiple vectors with very similar magnitude, just select the center sample
const float CENTER_TILE_BIAS = 1.01;

vec2 Max(vec2 a, vec2 b)
{
    return (dot(a, a) > dot(b, b)) ? a : b;
}

void main()
{
    // Select the longest velocity vector in a 3x3 sample.
    vec4 delta = u_VelocityTexelSize.xyxy * vec4(1.0, 1.0, -1.0, 0.0);

    vec2 veloTL = texture2D(u_VelocityTex, v_UV + delta.zy).rg;
    vec2 veloTM = texture2D(u_VelocityTex, v_UV + delta.wy).rg;
    vec2 veloTR = texture2D(u_VelocityTex, v_UV + delta.xy).rg;
    vec2 maxVelo = Max(Max(veloTL, veloTM), veloTR);

    vec2 veloML = texture2D(u_VelocityTex, v_UV + delta.zw).rg;
    vec2 veloM = texture2D(u_VelocityTex, v_UV).rg * CENTER_TILE_BIAS;
    vec2 veloMR = texture2D(u_VelocityTex, v_UV + delta.xw).rg;
    maxVelo = Max(maxVelo, Max(Max(veloML, veloM), veloMR));

    vec2 veloBL = texture2D(u_VelocityTex, v_UV - delta.xy).rg;
    vec2 veloBM = texture2D(u_VelocityTex, v_UV - delta.wy).rg;
    vec2 veloBR = texture2D(u_VelocityTex, v_UV - delta.zy).rg;

    fragColor = Max(maxVelo, Max(Max(veloBL, veloBM), veloBR));
}
