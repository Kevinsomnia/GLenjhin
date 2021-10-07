#pragma vertex
#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 u_VP;
uniform mat4 u_Model;

out vec3 wPos;
out vec3 wNormal;
out vec2 v_UV;

void main()
{
    vec4 worldPos = u_Model * aPosition;
    vec4 worldNormal = u_Model * vec4(aNormal, 0.0);

    wPos = worldPos.xyz;
    wNormal = worldNormal.xyz;

    gl_Position = u_VP * worldPos;
    v_UV = aUV;
}



#pragma fragment
#version 330 core

uniform vec3 u_CameraPos;
uniform vec3 u_DirLightDir;
uniform vec3 u_DirLightColor;

uniform sampler2D u_MainTex;

in vec3 wPos;
in vec3 wNormal;
in vec2 v_UV;

out vec4 color;

void main()
{
    vec4 tColor = texture(u_MainTex, v_UV);

    vec3 ambient = vec3(0.1);   // temp
    vec3 albedo = tColor.rgb;

    vec3 nrm = normalize(wNormal);

    float nDotL = max(0.0, dot(-u_DirLightDir, nrm));
    vec3 viewDir = normalize(wPos - u_CameraPos);
    vec3 halfDir = normalize(viewDir - u_DirLightDir);

    float specContrib = pow(max(0.0, dot(halfDir, nrm)), 6.0) * nDotL;
    vec3 specular = vec3(1.0) * specContrib;

    color = vec4((albedo * mix(ambient, vec3(1.0), nDotL)) + specular, 1.0);
}
