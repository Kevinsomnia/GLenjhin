#pragma vertex
#version 330 core

layout(location = 0) in vec4 position;
out vec4 pos;

void main()
{
	gl_Position = position;
	pos = position;
}



#pragma fragment
#version 330 core

in vec4 pos;
out vec4 color;

void main()
{
	color = vec4(0.2 + pos.x * 0.8, pos.y, 0.5 + pos.z * 0.5, 1.0);
}
