#pragma vertex
#version 330 core

layout(location = 0) in vec4 aPosition;

uniform mat4 u_VP;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_VP * u_Model * aPosition;
}



#pragma fragment
#version 330 core

out vec4 fragColor;

void main()
{
    vec3 color = vec3(1.25, 2.0, 0.75);
    fragColor = vec4(color, 1.0);
}
