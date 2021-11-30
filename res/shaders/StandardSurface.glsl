#pragma vertex
#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 u_VP;
uniform mat4 u_Model;

out vec3 v_Pos;
out vec3 v_Normal;
out vec2 v_UV;

void main()
{
    vec4 worldPos = u_Model * aPosition;

    v_Pos = worldPos.xyz;
    v_Normal = transpose(inverse(mat3(u_Model))) * aNormal;
    v_UV = vec2(aUV.x, 1.0 - aUV.y);

    gl_Position = u_VP * worldPos;
}



#pragma fragment
#version 330 core

uniform vec3 u_CameraPos;
uniform vec3 u_DirLightDir;
uniform vec3 u_DirLightColor;

uniform sampler2D u_MainTex;
uniform vec4 u_EmissionColor;

in vec3 v_Pos;
in vec3 v_Normal;
in vec2 v_UV;

out vec4 color;

// Forward rendering
void main()
{
    vec3 ambient = vec3(0.1);   // needs uniform
    vec3 nrm = normalize(v_Normal);
    float shininess = 128.0;

    // Lambert
    float nDotL = max(0.0, dot(-u_DirLightDir, nrm));

    // Blinn-Phong
    vec3 viewDir = normalize(u_CameraPos - v_Pos);
    vec3 halfDir = normalize(viewDir - u_DirLightDir);
    float specContrib = max(0.0, dot(halfDir, nrm));

    vec4 albedo = texture(u_MainTex, v_UV);
    float specular = pow(specContrib, shininess);
    color = vec4((albedo.rgb * mix(ambient, vec3(1.0), nDotL)) + specular + u_EmissionColor.rgb, 1.0);
}
