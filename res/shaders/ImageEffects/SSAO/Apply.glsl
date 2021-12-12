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

layout(location = 3) out vec4 gEmissionOcclusion;

uniform sampler2D u_EmissionOccl;
uniform sampler2D u_Occlusion;

in vec2 v_UV;

void main()
{
    vec4 emissionOcclusion = texture2D(u_EmissionOccl, v_UV);
    float occlusion = texture2D(u_Occlusion, v_UV).r;
    emissionOcclusion.a *= occlusion;
    gEmissionOcclusion = emissionOcclusion;
}
