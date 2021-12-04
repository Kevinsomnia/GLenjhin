#pragma vertex
#version 330 core

layout(location = 0) in vec4 aPosition;

uniform mat4 u_L;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_L * u_Model * aPosition;
}



#pragma fragment
#version 330 core

void main()
{
}
