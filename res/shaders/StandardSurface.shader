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
    wNormal = normalize(worldNormal.xyz);

    gl_Position = u_VP * worldPos;
    v_UV = vec2(aUV.x, 1.0 - aUV.y);
}



#pragma fragment
#version 330 core

uniform vec3 u_CameraPos;
uniform vec3 u_DirLightDir;
uniform vec3 u_DirLightColor;

uniform sampler2D u_MainTex;

in vec2 v_UV;

in vec3 wPos;
in vec3 wNormal;

out vec4 color;

void main()
{
    // needs uniform
    vec3 ambient = vec3(0.1);
    float shininess = 128.0;

    // Lambert
    float nDotL = max(0.0, dot(-u_DirLightDir, wNormal));

    // Blinn-Phong
    vec3 viewDir = normalize(u_CameraPos - wPos);
    vec3 halfDir = normalize(viewDir - u_DirLightDir);
    float specContrib = max(0.0, dot(halfDir, wNormal));

    vec4 albedo = texture(u_MainTex, v_UV);
    float specular = pow(specContrib, shininess);
    color = vec4((albedo.rgb * mix(ambient, vec3(1.0), nDotL)) + specular, 1.0);
}
