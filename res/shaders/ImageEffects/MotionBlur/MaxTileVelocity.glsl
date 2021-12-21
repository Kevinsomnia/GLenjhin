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
uniform vec2 u_TileParams;  // X = tile size, Y = iterations

in vec2 v_UV;

out vec2 fragColor;

vec2 Max(vec2 a, vec2 b)
{
    return (dot(a, a) > dot(b, b)) ? a : b;
}

void main()
{
    // Start sampling at the bottom-left corner of the mega-tile, and sample towards the top-right.
    vec2 tileUvStart = v_UV + (u_VelocityTexelSize * u_TileParams.xx);
    vec2 maxVelo = vec2(0.0);

    // Select the longest velocity vector in an entire mega-tile.
    for (int y = 0; y < u_TileParams.y; y++)
    {
        for (int x = 0; x < u_TileParams.y; x++)
        {
            vec2 uv = tileUvStart + (u_VelocityTexelSize * vec2(x, y));
            vec2 velo = texture2D(u_VelocityTex, uv).rg;
            maxVelo = Max(maxVelo, velo);
        }
    }

    fragColor = maxVelo;
}
