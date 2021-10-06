#pragma vertex
#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 u_VP;
uniform mat4 u_Model;

out vec3 wPos;
out vec3 wNormal;

void main()
{
    vec4 worldPos = u_Model * aPosition;
    vec4 worldNormal = u_Model * vec4(aNormal, 0.0);

    wPos = worldPos.xyz;
    wNormal = worldNormal.xyz;

    gl_Position = u_VP * worldPos;
}



#pragma fragment
#version 330 core

uniform vec3 u_CameraPos;
uniform vec3 u_DirLightDir;
uniform vec3 u_DirLightColor;

in vec3 wPos;
in vec3 wNormal;

out vec4 color;

void main()
{
    vec3 ambient = vec3(0.1);
    vec3 albedo = vec3(0.9, 0.625, 0.55);

    vec3 nrm = normalize(wNormal);

    float nDotL = max(0.0, dot(-u_DirLightDir, nrm));
    vec3 viewDir = normalize(wPos - u_CameraPos);
    vec3 halfDir = normalize(viewDir - u_DirLightDir);

    float specContrib = pow(max(0.0, dot(halfDir, nrm)), 6.0) * nDotL;
    vec3 specular = vec3(1.0) * specContrib;

    color = vec4(ambient + (albedo * nDotL) + specular, 1.0);
}
