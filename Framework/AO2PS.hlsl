#include "ShaderInfo.cginc"

#define AO_CHECK_LENGTH 0.5f
#define AO_INTENSITY_FADE_START_DISTZ 0.2f
#define AO_INTENSITY_FADE_END_DISTZ 2.0f

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 vFarPlanePos : TEXCOORD0;
    float2 tex : TEXCOORD1;
};

cbuffer CB_TRANSF_PROJUV : register(b0)
{
    float4x4 projUvMat;
}
cbuffer CB_OFFSET : register(b1)
{
    float4 offset[14];
}

Texture2D AOMap : register(t0);
SamplerState samp : register(s0);

float AOIntensity(float distZ)
{
    float intensity = 0;
    if(distZ > EPSILON)
    {
        float iLength = AO_INTENSITY_FADE_END_DISTZ - AO_INTENSITY_FADE_START_DISTZ;
        intensity = (AO_INTENSITY_FADE_END_DISTZ - distZ) / iLength;
    }
    return intensity;
}

float4 main(PS_INPUT input):SV_Target
{
    float4 vSample = AOMap.SampleLevel(samp, input.tex, 0);
    
    float3 vNormal = vSample.xyz;
    float vDepth = vSample.w;

    float3 vP = (vDepth / input.vFarPlanePos.z) * input.vFarPlanePos;

    float occlusionSum = 0;
    for (int i = 0; i < 14; ++i)
    {
        float reflectOffset = reflect(normalize(offset[i].xyz), vNormal);
        float flip = sign(dot(reflectOffset, vNormal));
        float3 vQ = vP + flip * reflectOffset * AO_CHECK_LENGTH;
        float4 pQ = mul(projUvMat, float4(vQ, 1));

        // when pQ.w == 0, uv for vQSample would be 0, which is (-1,-1) of projection space
        clip(pQ.w - EPSILON);
        float4 vQSample = AOMap.SampleLevel(samp, pQ.xy / pQ.w, 0);
        float3 vR = (vQSample.w / vQ.z) * vQ;
        
        float distZ = vP.z - vR.z;
        float dp = saturate(dot(vNormal, normalize(vR - vP)));
        occlusionSum += dp * AOIntensity(distZ);
    }

    float access = 1 - occlusionSum / 14;

    return access;
}