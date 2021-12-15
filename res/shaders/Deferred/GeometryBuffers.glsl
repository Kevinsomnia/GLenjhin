#pragma vertex
#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aUV;

uniform mat4 u_VP;
uniform mat4 u_Model;

out vec3 v_Position;
out vec3 v_Normal;
out vec3 v_Tangent;
out vec2 v_UV;

void main()
{
    vec4 worldPos = u_Model * aPosition;
    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));

    v_Position = worldPos.xyz;
    v_Normal = normalMatrix * aNormal;
    v_Tangent = normalMatrix * aTangent;
    v_UV = vec2(aUV.x, 1.0 - aUV.y);

    gl_Position = u_VP * worldPos;
}



#pragma fragment
#version 330 core

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormalSmoothness;
layout(location = 2) out vec4 gAlbedoMetallic;
layout(location = 3) out vec4 gEmissionOcclusion;

in vec3 v_Position;
in vec3 v_Normal;
in vec3 v_Tangent;
in vec2 v_UV;

uniform sampler2D u_AlbedoTex;
uniform sampler2D u_NormalTex;
uniform sampler2D u_MSATex;
uniform vec2 u_TileSize;
uniform vec4 u_EmissionColor;

void main()
{
    vec2 uv = v_UV * u_TileSize;

    vec3 albedo = texture2D(u_AlbedoTex, uv).rgb;
    vec3 nrm = texture2D(u_NormalTex, uv).rgb;
    vec3 msa = texture2D(u_MSATex, uv).rgb;

    // Create the world-space TBN for normal-map calculations. Use Gram-Schmidt process to reorthogonalize.
    vec3 tangent = normalize(v_Tangent);
    vec3 normal = normalize(v_Normal);
    tangent = normalize(tangent - (normal * dot(normal, tangent)));
    vec3 bitangent = cross(normal, tangent);
    mat3 worldTBN = mat3(tangent, bitangent, normal);

    // Construct the final world-space normal by applying the tangent-space normal vector onto the world TBN.
    vec3 tangentOffset = (nrm * 2.0) - 1.0;
    vec3 finalWorldNormal = normalize(worldTBN * tangentOffset);

    gPosition = vec4(v_Position, 1.0);
    gNormalSmoothness = vec4(finalWorldNormal, msa.g);
    gAlbedoMetallic = vec4(albedo, msa.r);
    gEmissionOcclusion = vec4(u_EmissionColor.rgb, msa.b);
}
