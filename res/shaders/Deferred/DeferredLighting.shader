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
uniform sampler2D u_Normal;
uniform sampler2D u_AlbedoSpec;

uniform vec3 u_CameraPos;
uniform vec3 u_DirLightDir;
uniform vec3 u_DirLightColor;

in vec2 v_UV;

out vec4 fragColor;

const float SHININESS = 128.0;

void main()
{
    vec4 pos = texture2D(u_Position, v_UV);
    vec4 nrm = texture2D(u_Normal, v_UV);
    vec4 albedoSpec = texture2D(u_AlbedoSpec, v_UV);

    vec3 ambient = vec3(0.1);   // needs uniform
    float nDotL = max(0.0, dot(-u_DirLightDir, nrm.xyz));

    vec3 viewDir = normalize(u_CameraPos - pos.xyz);
    vec3 halfDir = normalize(viewDir - u_DirLightDir);
    float specContrib = max(0.0, dot(halfDir, nrm.xyz));
    float specular = pow(specContrib, SHININESS);

    fragColor = vec4(albedoSpec.rgb * mix(ambient, vec3(1.0), nDotL) + specular, 1.0);
}
