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

uniform sampler2D u_Position;
uniform sampler2D u_NormalSmooth;
uniform sampler2D u_AlbedoMetal;
uniform sampler2D u_Emission;

uniform vec3 u_CameraPos;
uniform vec3 u_DirLightDir;
uniform vec3 u_DirLightColor;

in vec2 v_UV;

out vec4 fragColor;

const float SHININESS = 128.0;

void main()
{
    vec4 position = texture2D(u_Position, v_UV);
    vec4 normalsSmoothness = texture2D(u_NormalSmooth, v_UV);
    vec4 albedoMetallic = texture2D(u_AlbedoMetal, v_UV);
    vec4 emission = texture2D(u_Emission, v_UV);

    vec3 ambient = vec3(0.1);   // needs uniform
    float nDotL = max(0.0, dot(-u_DirLightDir, normalsSmoothness.xyz));

    vec3 viewDir = normalize(u_CameraPos - position.xyz);
    vec3 halfDir = normalize(viewDir - u_DirLightDir);
    float specContrib = max(0.0, dot(halfDir, normalsSmoothness.xyz));
    float specular = pow(specContrib, SHININESS);

    fragColor = vec4(albedoMetallic.rgb * mix(ambient, vec3(1.0), nDotL) + specular + emission.rgb, 1.0);
}
