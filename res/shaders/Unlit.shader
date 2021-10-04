#pragma vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_VP;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_VP * u_Model * position;
}



#pragma fragment
#version 330 core

out vec4 color;

void main()
{
	color = vec4(0.2, 0.7, 0.5, 1.0);
}
