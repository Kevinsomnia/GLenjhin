#pragma vertex
#version 330 core

layout(location = 0) in vec2 aPosition;

out vec2 v_UV;

void main()
{
    gl_Position = vec4(aPosition, 0.0, 1.0);
    v_UV = vec2(aPosition.x + 1.0, aPosition.y + 1.0) * 0.5;
}



#pragma fragment
#version 330 core

uniform sampler2D u_MainTex;
uniform sampler2D u_Depth;

uniform float u_Threshold;
uniform vec3 u_SunPos;  // viewport position

in vec2 v_UV;

out vec4 fragColor;

void main()
{
    float depth = texture2D(u_Depth, v_UV).r;
    float val = 0.0;

    if (depth >= 1.0)
    {
        vec4 screen = texture2D(u_MainTex, v_UV);
        float lumin = dot(screen.rgb, vec3(0.299, 0.587, 0.114));
        float threshold = max(0.0, lumin - u_Threshold);
        float dist = max(0.0, 1.0 - length(u_SunPos.xy - v_UV)) * step(0.0, u_SunPos.z);

        val = threshold * dist;
    }

    fragColor = vec4(vec3(val), 1.0);
}
