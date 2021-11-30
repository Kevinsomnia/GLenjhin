#pragma vertex
#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 u_VP;
uniform mat4 u_Model;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_UV;

void main()
{
    vec4 worldPos = u_Model * aPosition;

    v_Position = worldPos.xyz;
    v_Normal = transpose(inverse(mat3(u_Model))) * aNormal;
    v_UV = vec2(aUV.x, 1.0 - aUV.y);

    gl_Position = u_VP * worldPos;
}



#pragma fragment
#version 330 core

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_UV;

uniform sampler2D u_AlbedoTex;

void main()
{
    gPosition = vec4(v_Position, 1.0);
    gNormal = vec4(normalize(v_Normal), 1.0);
    gAlbedoSpec = vec4(texture2D(u_AlbedoTex, v_UV).rgb, 1.0);
}
