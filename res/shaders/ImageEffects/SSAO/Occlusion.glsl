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

uniform sampler2D u_Position;
uniform sampler2D u_NormalSmooth;
uniform sampler2D u_Depth;

uniform sampler2D u_Noise;
uniform vec2 u_NoiseTexelSize;

uniform vec2 u_TexelSize;
uniform float u_Intensity;
uniform float u_Radius;
uniform mat4 u_V;
uniform mat4 u_P;

in vec2 v_UV;

const float BIAS = 0.0001;

// Random samples in unit hemisphere, but with a biased towards the origin (r^2)
#define SAMPLE_COUNT 16
vec3 sampleOffsets[SAMPLE_COUNT] = vec3[] (
    vec3(-0.4057631, -0.0496792, 0.8340108), vec3(-0.1136972, 0.2621384, 0.1772420), vec3(-0.0704513, 0.5152024, 0.3575099), vec3(-0.1845606, 0.3780357, 0.0396810),
    vec3(-0.2546581, 0.0125041, 0.4660064), vec3(0.1796711, 0.0045031, 0.1257799), vec3(-0.8259325, -0.1608233, 0.4500470), vec3(-0.0492182, 0.5716730, 0.1234637),
    vec3(-0.3980426, -0.0888574, 0.1349369), vec3(-0.0122239, -0.4266508, 0.0846760), vec3(-0.0613640, -0.3013219, 0.3677131), vec3(-0.0620663, 0.0645950, 0.3933732),
    vec3(0.7568239, -0.5376369, 0.1954271), vec3(-0.0103794, -0.0267645, 0.0129755), vec3(0.1056587, -0.3673950, 0.3941123), vec3(0.0451745, -0.2339309, 0.0194765)
);

out vec4 fragColor;

void main()
{
    float depth = texture2D(u_Depth, v_UV).r;
    float occlusion;

    // We can only calculate occlusion for stuff that's within our camera frustum.
    if (depth < 1.0)
    {
        vec3 worldPos = texture2D(u_Position, v_UV).rgb;
        vec3 worldNormal = texture2D(u_NormalSmooth, v_UV).rgb;

        // Fixup precision issues (especially at extremely high smoothness) by renormalizing.
        worldNormal = normalize(worldNormal);

        // We need calculations to be in view-space since depth is in view-space.
        vec4 viewPos = u_V * vec4(worldPos, 1.0);
        vec3 viewNormal = mat3(u_V) * worldNormal;

        // Use random noise in screen-space to dither and eliminate banding artifacts.
        vec2 noiseScale = vec2(1.0) / (u_TexelSize / u_NoiseTexelSize);
        vec2 noise = texture2D(u_Noise, v_UV * noiseScale).rg * 2.0 - 1.0; // [-1, 1]

        // We will need to get the orthonormal basis vectors oriented along the surface normal, in view space.
        // We use this to transform our tangent-space hemisphere samples.
        vec3 tangentBase = vec3(normalize(noise), 0.0);
        vec3 viewTangent = normalize(tangentBase - (viewNormal * dot(tangentBase, viewNormal)));
        vec3 viewBitangent = cross(viewNormal, viewTangent);

        // Create the hemisphere rotation matrix using the 3 vectors that make it up.
        // We don't need a 4x4 matrix since we're transforming directions only.
        mat3 tbn = mat3(viewTangent, viewBitangent, viewNormal);

        // Higher values = greater contribution of occlusion rather than a multiplier. This will make things more intuitive.
        occlusion = 0.0;

        for (int i = 0; i < SAMPLE_COUNT; i++)
        {
            vec3 viewSampleDir = (tbn * sampleOffsets[i]) * u_Radius;
            // Rebuild the hemisphere sample point in view space.
            vec3 viewSamplePos = viewPos.xyz + viewSampleDir;

            // Convert the viewSamplePos to viewport coordinates (0 to 1).
            // This is so we can properly sample the world position buffer and get the world depth (Z) directly behind that view sample.
            // Of course, we would then need to convert that world-space depth to view-space so we can correctly compare against viewSamplePos to see if the sample is intersecting.
            vec4 clipPos = u_P * vec4(viewSamplePos, 1.0);
            clipPos.xy /= clipPos.w;
            clipPos.xy = clipPos.xy * 0.5 + 0.5;

            // Essentially fetches the world position along the ray towards the view-space hemisphere sample.
            vec3 rayWorldPos = texture2D(u_Position, clipPos.xy).rgb;

            // Ignore samples that are in front of "nothing", otherwise they would lead to invalid occlusion results. We approximate this by checking for the position GBuffer's clear color (0, 0, 0).
            // Sampling the NDC depth won't give us accurate results, unlike the first depth check.
            if (dot(rayWorldPos, rayWorldPos) > 0.0)
            {
                // Transform the Z depth of this world ray into view space. TODO: only Z coord is needed
                vec4 rayViewPos = u_V * vec4(rayWorldPos, 1.0);

                // 1.0 if this viewSamplePos is being occluded behind a surface (rayViewPos), otherwise 0.0.
                float contrib = (-viewSamplePos.z > -rayViewPos.z + BIAS) ? 1.0 : 0.0;
                // We would need to test both ray view depth and the fragment's view depth, to check whether or not it's in range of the hemisphere radius.
                // This is to handle cases where, for example, the hemisphere normal is pointing orthogonal (camera looking through the side of the hemisphere).
                // Also applies a nice inverse falloff based on depth difference.
                float depthCheck = clamp(u_Radius / abs(-rayViewPos.z - -viewPos.z), 0.0, 1.0);
                occlusion += contrib * depthCheck;
            }
        }

        // Convert the contribution value back to a multiply color by averaging and inverting the result.
        occlusion = clamp(1.0 - (occlusion * u_Intensity / SAMPLE_COUNT), 0.0, 1.0);
    }
    else
    {
        occlusion = 1.0;
    }

    fragColor = vec4(occlusion, 0.0, 0.0, 1.0);
}
