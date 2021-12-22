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

const float PI = 3.141592653589793;

uniform sampler2D u_MainTex;
uniform sampler2D u_PackedTex;
uniform vec2 u_PackedTexSize;
uniform vec2 u_PackedTexelSize;
uniform sampler2D u_MaxNeighborTex;
uniform vec2 u_MaxNeighborTexelSize;
uniform float u_MaxBlurRadius;
uniform float u_SampleCount;
uniform float u_Time;

in vec2 v_UV;

out vec4 fragColor;

bool Interval(float v, float count)
{
    return fract(v / count) > 0.4999;
}

// Jorge Jimenez: SIGGRAPH 2014 - Interleaved gradient noise
float InterleavedGradientNoise(vec2 uv)
{
    uv = floor((uv + u_Time) * u_PackedTexSize);
    float noise = fract(dot(vec2(0.06711056, 0.00583715), uv));
    return fract(52.9829189 * noise);
}

// Jitter for tile lookup
vec2 JitterTile(vec2 uv)
{
    float theta = InterleavedGradientNoise(v_UV + vec2(1.0, 0.0)) * 2.0 * PI;
    return uv + (vec2(cos(theta), sin(theta)) * u_MaxNeighborTexelSize * 0.25);
}

vec3 SampleVelocityDepth(vec2 uv)
{
    vec3 packedVal = texture2D(u_PackedTex, uv).rgb;
    return vec3((packedVal.rg * 2.0 - 1.0) * u_MaxBlurRadius, 1.0 / packedVal.b);
}

void main()
{
    vec4 screen = texture2D(u_MainTex, v_UV);
    vec2 veloMax = texture2D(u_MaxNeighborTex, JitterTile(v_UV)).rg;
    float veloMaxMagn = length(veloMax);

    // Don't bother processing any tiles that have barely moved.
    if (veloMaxMagn < 2.0)
    {
        fragColor = screen;
        return;
    }

    vec3 centerVeloDepth = SampleVelocityDepth(v_UV);
    // How many pixels has this fragment moved since last frame? We want to clip off values below 0.5 pixels.
    float centerVeloMagn = max(0.5, length(centerVeloDepth.xy));
    // Use the pixel velocity of the center sample, to approximate blurring whatever's behind that fragment
    float veloMagnBG = max(1.0, centerVeloMagn);

    // Normalize center pixel velocity to be the same length as veloMax, and use that as a secondary direction, but only if it's at least twice the max magnitude.
    vec2 veloAlt = (centerVeloMagn * 2.0 > veloMaxMagn) ? centerVeloDepth.xy * (veloMax / centerVeloMagn) : veloMax;

    float sampleCount = floor(min(veloMaxMagn * 0.5, u_SampleCount));
    float stepSize = 1.0 / sampleCount;
    float sample = 1.0 - (stepSize * 0.5);
    float i = 0.0;
    // Randomly shift the sampling distance off center per-fragment, to heavily reduce color banding.
    float randomOffs = (InterleavedGradientNoise(v_UV) - 0.5) * stepSize;

    // Accumulated color (RGB), accumulated weight (A)
    vec4 accum = vec4(0.0);

    // Sample from edge to center.
    while (sample > stepSize * 0.25)
    {
        // Sample direction is swapped every 2 iterations.
        vec2 sampleDir = Interval(i, 4.0) ? veloAlt : veloMax;
        // Sample offset is inverted after every iteration (start with positive sample).
        float sampleOffs = (Interval(i, 2.0) ? -sample : sample) + randomOffs;

        // Compute sample UV
        vec2 uv = v_UV + (sampleDir * sampleOffs * u_PackedTexelSize);

        // Sample pixel color
        vec3 col = texture2D(u_MainTex, uv).rgb;

        // Sample motion
        vec3 sampleVeloDepth = SampleVelocityDepth(uv);
        float sampleVeloMagn = length(sampleVeloDepth.xy);

        // Blend between background/foreground velocity using depth differences.
        // We approximate the background pixels (the fragments occluded behind this fragment) based on the nearby pixels.
        float foreground = clamp((centerVeloDepth.z - sampleVeloDepth.z) * 20.0 / centerVeloDepth.z, 0.0, 1.0);
        sampleVeloMagn = mix(veloMagnBG, sampleVeloMagn, foreground);

        // Sample weight: distance test * motion * triangular window.
        float sampleDist = veloMaxMagn * abs(sample);
        float weight = clamp(sampleVeloMagn - sampleDist, 0.0, 1.0) / sampleVeloMagn * (1.2 - sample);

        accum += vec4(col, 1.0) * weight;

        if (Interval(i, 2.0))
            sample -= stepSize;
        i += 1.0;
    }

    // Finally the center pixel w/ proper weight (no distance test).
    accum += vec4(screen.rgb, 1.0) * (1.2 / (veloMagnBG * sampleCount * 2.0));

    fragColor = vec4(accum.rgb / accum.a, screen.a);
}
