#pragma vertex
#version 330 core

layout(location = 0) in vec4 position;
out vec4 pos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * position;
	pos = position;
}



#pragma fragment
#version 330 core

in vec4 pos;
out vec4 color;

void main()
{
	color = vec4(0.5 + pos.x * 0.8, 0.5 + pos.y * 0.8, 0.5 + pos.z * 0.8, 1.0);
}
