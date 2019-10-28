#include "ShaderInfo.cginc"

#define AO_CHECK_LENGTH 20.0f
#define AO_INTENSITY_START_DISTZ 10.0f
#define AO_INTENSITY_MAX_DISTZ 2.0f

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

Texture2D AOMap : register(t0);
SamplerState samp : register(s0);

float AOIntensity(float distZ)
{
    float intensity = 0;
    if(distZ > EPSILON)
    {
        float iLength = AO_INTENSITY_START_DISTZ - AO_INTENSITY_MAX_DISTZ;
        intensity = (AO_INTENSITY_START_DISTZ - distZ) / iLength;
    }
    return intensity;
}

float4 main(PS_INPUT input):SV_Target
{
    float4 vSample = AOMap.SampleLevel(samp, input.tex, 0);
    float3 vNormal = vSample.xyz;
    float vDepth = vSample.w;

    float3 vP = (vDepth / input.vFarPlanePos.z) * input.vFarPlanePos;
    float3 vQ = vP + vNormal * AO_CHECK_LENGTH;
    float4 pQ = mul(projUvMat, float4(vQ, 1));
    float4 vQSample = AOMap.SampleLevel(samp, pQ.xy / pQ.w, 0);
    float3 vR = (vQSample.w / vQ.z)*vQ;

    float distZ = vP.z - vR.z;
    float dp = saturate(dot(vNormal, normalize(vR - vP)));
    float occlusionSum = dp * AOIntensity(distZ);

    return occlusionSum;
}