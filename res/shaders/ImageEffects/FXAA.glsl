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
uniform vec2 u_TexelSize;

in vec2 v_UV;

out vec4 fragColor;

const vec3 LUMA = vec3(0.299, 0.587, 0.114);

// Minimum amount of local contrast required to apply anti-aliasing.
const float EDGE_THRESHOLD = 1.0 / 12.0;
// Skips processing dark luma values where the contrast is too low to see a difference.
const float EDGE_THRESHOLD_MIN = 1.0 / 48.0;
// Controls when to stop searching span. 0.25 seems to be the best quality wise.
const float SEARCH_THRESHOLD = 0.25;
// Max number of search steps for end of span.
const int SEARCH_STEPS = 16;
// Insures fine detail is not completely removed. Higher values will increase the sensitivity to subpixel aliasing removal.
// This is important for the transition of sub-pixel detail, like fences and wires.
const float SUBPIX_CAP = 3.0 / 4.0;
// Controls sub-pixel aliasing removal. Lower values tend towards more removal. Recommended: 1.0 / 4.0
const float SUBPIX_TRIM = 1.0 / 4.0;


// Other constants.
const float SUBPIX_TRIM_SCALE = 1.0 / (1.0 - SUBPIX_TRIM);


void main()
{
    // Sample pixel, and the N, W, E, S neighbors.
    vec4 delta = u_TexelSize.xyxy * vec4(1.0, 1.0, -1.0, 0.0);
    vec3 sampleM = texture2D(u_MainTex, v_UV).rgb;
    vec3 sampleN = texture2D(u_MainTex, v_UV + delta.wy).rgb;
    vec3 sampleW = texture2D(u_MainTex, v_UV - delta.xw).rgb;
    vec3 sampleE = texture2D(u_MainTex, v_UV + delta.xw).rgb;
    vec3 sampleS = texture2D(u_MainTex, v_UV - delta.wy).rgb;
    float lumaM = dot(sampleM, LUMA);
    float lumaN = dot(sampleN, LUMA);
    float lumaW = dot(sampleW, LUMA);
    float lumaE = dot(sampleE, LUMA);
    float lumaS = dot(sampleS, LUMA);

    // Calculate local contrast.
    float lumaMin = min(lumaM, min(min(lumaN, lumaS), min(lumaW, lumaE)));
    float lumaMax = max(lumaM, max(max(lumaN, lumaS), max(lumaW, lumaE)));
    float lumaRange = lumaMax - lumaMin;

    // Early exit if the lumaRange (local contrast) is outside of edge detection threshold
    if (lumaRange < max(EDGE_THRESHOLD_MIN, lumaMax * EDGE_THRESHOLD))
    {
        fragColor = vec4(sampleM, 1.0);
        return;
    }

    // Basic low-pass value: (N + W + E + S) / 4.0
    // blendLowPass will be used to blend between the screen color and anti-aliased result
    float lumaLowPass = (lumaN + lumaW + lumaE + lumaS) * 0.25;
    float lumaRangeLowPass = abs(lumaLowPass - lumaM);
    float blendLowPass = max(0.0, (lumaRangeLowPass / lumaRange) - SUBPIX_TRIM) * SUBPIX_TRIM_SCALE;
    blendLowPass = min(blendLowPass, SUBPIX_CAP);

    // Compute low-pass value in a 3x3 local neighborhood: (N + W + E + S + M + NW + NE + SW + SE) / 9.0
    vec3 sampleNW = texture2D(u_MainTex, v_UV + delta.zy).rgb;
    vec3 sampleNE = texture2D(u_MainTex, v_UV + delta.xy).rgb;
    vec3 sampleSW = texture2D(u_MainTex, v_UV - delta.xy).rgb;
    vec3 sampleSE = texture2D(u_MainTex, v_UV - delta.zy).rgb;
    float lumaNW = dot(sampleNW, LUMA);
    float lumaNE = dot(sampleNE, LUMA);
    float lumaSW = dot(sampleSW, LUMA);
    float lumaSE = dot(sampleSE, LUMA);

    vec3 sampleLowPass = (sampleM + sampleN + sampleW + sampleE + sampleS + sampleNW + sampleNE + sampleSW + sampleSE) / 9.0;

    // Sampling vertically to detect horizontal edges.
    float edgeHori = abs((lumaNW * 0.25) + (lumaW * -0.5) + (lumaSW * 0.25)) +
                     abs((lumaN * 0.5) + (lumaM * -1.0) + (lumaS * 0.5)) +
                     abs((lumaNE * 0.25) + (lumaE * -0.5) + (lumaSE * 0.25));
    // Sampling horizontally to detect vertical edges.
    float edgeVert = abs((lumaNW * 0.25) + (lumaN * -0.5) + (lumaNE * 0.25)) +
                     abs((lumaW * 0.5) + (lumaM * -1.0) + (lumaE * 0.5)) +
                     abs((lumaSW * 0.25) + (lumaS * -0.5) + (lumaSE * 0.25));
    bool horiSpan = edgeHori >= edgeVert;
    float lengthSign = horiSpan ? -u_TexelSize.y : -u_TexelSize.x;

    if (horiSpan)
    {
        lumaN = lumaW;
        lumaS = lumaE;
    }

    float gradientN = abs(lumaN - lumaM);
    float gradientS = abs(lumaS - lumaM);
    lumaN = (lumaN + lumaM) * 0.5;
    lumaS = (lumaS + lumaM) * 0.5;

    // Choose side where the gradient is highest.
    if (gradientN < gradientS)
    {
        lumaN = lumaS;
        gradientN = gradientS;
        lengthSign *= -1.0;
    }

    // Subpixel UV coordinate.
    vec2 uvNeg = vec2(
        v_UV.x + (horiSpan ? 0.0 : lengthSign * 0.5),
        v_UV.y + (horiSpan ? lengthSign * 0.5 : 0.0)
    );

    gradientN *= SEARCH_THRESHOLD;

    // Search in both positive and negative side of a horizontal/vertical direction, until the luma pair average is out of range.
    vec2 uvPos = uvNeg;
    vec2 offs = horiSpan ? vec2(u_TexelSize.x, 0.0) : vec2(0.0, u_TexelSize.y);
    float lumaEndPos = lumaN;
    float lumaEndNeg = lumaN;
    bool donePos = false;
    bool doneNeg = false;
    uvPos += offs;
    uvNeg -= offs;

    for (int i = 0; i < SEARCH_STEPS; i++)
    {
        if (!donePos)
            lumaEndPos = dot(texture2D(u_MainTex, uvPos).rgb, LUMA);
        if (!doneNeg)
            lumaEndNeg = dot(texture2D(u_MainTex, uvNeg).rgb, LUMA);

        donePos = donePos || (abs(lumaEndPos - lumaN) >= gradientN);
        doneNeg = doneNeg || (abs(lumaEndNeg - lumaN) >= gradientN);

        if (donePos && doneNeg)
            break;

        if (!donePos)
            uvPos += offs;
        if (!doneNeg)
            uvNeg -= offs;
    }

    // Distances from center sample.
    float distPos = horiSpan ? uvPos.x - v_UV.x : uvPos.y - v_UV.y;
    float distNeg = horiSpan ? v_UV.x - uvNeg.x : v_UV.y - uvNeg.y;
    bool directionNeg = distNeg < distPos;
    if (!directionNeg)
        lumaEndNeg = lumaEndPos;

    // Check if the pixel is in a section of span which gets no filtering.
    if (((lumaM - lumaN) < 0.0) == ((lumaEndNeg - lumaN) < 0.0))
        lengthSign = 0.0;

    float spanLength = distPos + distNeg;
    if (!directionNeg)
        distNeg = distPos;
    float subPixelOffs = (0.5 + (distNeg * (-1.0 / spanLength))) * lengthSign;

    vec2 filterUV = vec2(
        v_UV.x + (horiSpan ? 0.0 : subPixelOffs),
        v_UV.y + (horiSpan ? subPixelOffs : 0.0)
    );
    vec3 sampleFiltered = texture2D(u_MainTex, filterUV).rgb;

    fragColor = vec4(mix(sampleFiltered, sampleLowPass, blendLowPass), 1.0);
}
