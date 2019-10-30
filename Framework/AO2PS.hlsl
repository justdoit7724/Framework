
#include "ShaderInfo.cginc"

#define SAMPLE_LENGTH 1.5f
#define FADE_START 0.2f
#define FADE_END 2.0f

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

Texture2D aoMap : register(t0);
SamplerState samp : register(s0);

float AOIntensity(float distZ)
{
    float intensity = 0;
    if(distZ >EPSILON)
    {
        float fadeLength = FADE_END - FADE_START;
        intensity = saturate((FADE_END - distZ) / fadeLength);
    }
    return intensity;
}

float4 main(PS_INPUT input) :SV_Target
{
    float4 vSample = aoMap.SampleLevel(samp, input.tex, 0);
    if(sign(input.vFarPlanePos.z-vSample.w)<0)
        return float4(1, 1, 1, 1);
    float3 vNormal = vSample.xyz;
    float vDepth = vSample.w;

    float3 vP = (vDepth / input.vFarPlanePos.z) *input.vFarPlanePos;
    float3 vQ = vP + vNormal * SAMPLE_LENGTH;
    float4 pQ = mul(projUvMat, float4(vQ, 1));
    float2 vQUV = pQ.xy / pQ.w;
    float4 vRSample = aoMap.SampleLevel(samp, vQUV, 0);
    float vrDepth = vRSample.w;

    float3 vR = (vrDepth / vQ.z) * vQ;
    float distZ = vP.z - vR.z;
    float dp = saturate(dot(vNormal, normalize(vR - vP)));
    
    float occlusionSum = dp*AOIntensity(distZ);

    float access = 1 - occlusionSum;
    return float4(access.xxx, 1);

}