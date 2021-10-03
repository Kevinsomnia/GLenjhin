#pragma vertex
#version 330 core

layout(location = 0) in vec4 position;
out vec4 pos;

uniform mat4 u_Model;
uniform mat4 u_View;

void main()
{
	// PROJECTION
	float fov = tan(75.0 * 0.01745329251 * 0.5);
	const float aspect = 1.7777777;

	mat4 proj;
	proj[0] = vec4(1.0 / (fov * aspect), 0.0, 0.0, 0.0);
	proj[1] = vec4(0.0, 1.0 / fov, 0.0, 0.0);
	proj[2] = vec4(0.0, 0.0, 0.0, 1.0);
	proj[3] = vec4(0.0, 0.0, 0.0, 0.0);

	gl_Position = proj * u_View * u_Model * position;
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
