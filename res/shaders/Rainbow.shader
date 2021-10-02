#pragma vertex
#version 330 core

layout(location = 0) in vec4 position;
out vec4 pos;

void main()
{
	mat4 mPosition;
	vec3 tPos = vec3(-0.85, -0.85, 3.5);
	vec3 tScale = vec3(1.0, 1.0, 1.0);
	mPosition[0] = vec4(1.0, 0.0, 0.0, 0.0);
	mPosition[1] = vec4(0.0, 1.0, 0.0, 0.0);
	mPosition[2] = vec4(0.0, 0.0, 1.0, 0.0);
	mPosition[3] = vec4(tPos, 1.0);
	mat4 mScale;
	mScale[0] = vec4(tScale.x, 0.0, 0.0, 0.0);
	mScale[1] = vec4(0.0, tScale.y, 0.0, 0.0);
	mScale[2] = vec4(0.0, 0.0, tScale.z, 0.0);
	mScale[3] = vec4(0.0, 0.0, 0.0, 1.0);
	mat4 model = mPosition * mScale;

	mat4 viewPos;
	vec3 cPos = vec3(0.0, 0.0, 0.0);
	viewPos[0] = vec4(1.0, 0.0, 0.0, 0.0);
	viewPos[1] = vec4(0.0, 1.0, 0.0, 0.0);
	viewPos[2] = vec4(0.0, 0.0, 1.0, 0.0);
	viewPos[3] = vec4(-cPos, 1.0);
	mat4 viewRot;
	viewRot[0] = vec4(1.0, 0.0, 0.0, 0.0);
	viewRot[1] = vec4(0.0, 1.0, 0.0, 0.0);
	viewRot[2] = vec4(0.0, 0.0, 1.0, 0.0);
	viewRot[3] = vec4(0.0, 0.0, 0.0, 1.0);
	mat4 view = viewRot * viewPos;

	float fov = tan(75.0 * 0.01745329251 * 0.5);
	float aspect = 1.777;
	mat4 proj;
	proj[0] = vec4(1.0 / (fov * aspect), 0.0, 0.0, 0.0);
	proj[1] = vec4(0.0, 1.0 / fov, 0.0, 0.0);
	proj[2] = vec4(0.0, 0.0, 0.0, 1.0);
	proj[3] = vec4(0.0, 0.0, 0.0, 0.0);

	gl_Position = proj * view * model * position;
	pos = position;
}



#pragma fragment
#version 330 core

in vec4 pos;
out vec4 color;

void main()
{
	color = vec4(0.5 + pos.x * 0.5, 0.5 + pos.y * 0.5, 0.5 + pos.z * 0.5, 1.0);
}
