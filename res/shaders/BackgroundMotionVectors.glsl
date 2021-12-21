#pragma vertex
#version 330 core

layout(location = 0) in vec2 aPosition;

uniform mat4 u_PrevVP;
uniform mat4 u_CurrVP;

out vec4 v_PrevPosNDC;
out vec2 v_CurrPosNDC;

void main()
{
    vec4 ndcPos = vec4(aPosition, 1.0, 1.0);
    // World-space positions of frustum corners
    vec4 worldPos = inverse(u_CurrVP) * ndcPos;

    // Convert from world-space to previous frame's NDC coordinate space.
    v_PrevPosNDC = u_PrevVP * worldPos;
    // No need to convert, `aPosition` is conveniently the value that we want since u_CurrVP * inverse(u_CurrVP) = I.
    v_CurrPosNDC = aPosition;

    // NDC is scaled from [-1, 1], so we need to halve the values to normalize the delta to viewport scale [0, 1].
    v_PrevPosNDC.xy *= 0.5;
    v_CurrPosNDC *= 0.5;

    gl_Position = ndcPos;
}



#pragma fragment
#version 330 core

layout(location = 4) out vec2 gMotionVectors;

in vec4 v_PrevPosNDC;
in vec2 v_CurrPosNDC;

void main()
{
    vec2 prevPos = v_PrevPosNDC.xy / v_PrevPosNDC.w;
    gMotionVectors = v_CurrPosNDC - prevPos;
}
