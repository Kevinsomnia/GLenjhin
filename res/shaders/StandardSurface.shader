#pragma vertex
#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 u_VP;
uniform mat4 u_Model;

out vec3 vNormal;

void main()
{
	gl_Position = u_VP * u_Model * aPosition;
	vec4 worldNormal = u_Model * vec4(aNormal, 0.0);
	vNormal = worldNormal.xyz;
}



#pragma fragment
#version 330 core

uniform vec3 u_DirLightDir;
uniform vec3 u_DirLightColor;

in vec3 vNormal;

out vec4 color;

void main()
{
	float nDotL = max(0.0, dot(-u_DirLightDir, vNormal));
	vec3 ambient = vec3(0.1);
	vec3 albedo = vec3(1.0);
	color = vec4(mix(ambient, albedo, nDotL), 1.0);
}
