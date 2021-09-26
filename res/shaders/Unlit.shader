#pragma vertex
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
}



#pragma fragment
#version 330 core

out vec4 color;

void main()
{
	color = vec4(0.2, 0.7, 0.5, 1.0);
}