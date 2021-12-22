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

uniform sampler2D u_MotionTex;
uniform sampler2D u_DepthTex;
uniform vec3 u_MotionParams;    // XY = motion texture size, Z = max blur radius
uniform vec4 u_ProjectionParams;
uniform float u_VelocityScale;

in vec2 v_UV;

out vec4 fragColor;

// Convert NDC depth to a linear value: 0 = near, 1 = far
float LinearDepth01(float depth)
{
    float ndcZ = depth * 2.0 - 1.0;
    // (2 * near) / (near + far - ndcZ * (far - near))
    return (2.0 * u_ProjectionParams.x) / (u_ProjectionParams.x + u_ProjectionParams.y - ndcZ * u_ProjectionParams.z);
}

void main()
{
    vec2 velo = texture2D(u_MotionTex, v_UV).rg * u_VelocityScale;
    // Convert motion texture from viewport to pixel-space.
    velo *= u_MotionParams.xy;
    // Clamp the velocity to a maximum blur radius in pixels.
    velo /= max(1.0, length(velo) / u_MotionParams.z);
    // Normalize values to [0, 1]
    velo = velo * (0.5 / u_MotionParams.z) + 0.5;

    float linDepth = LinearDepth01(texture2D(u_DepthTex, v_UV).r);
    fragColor = vec4(velo, linDepth, 1.0);
}
