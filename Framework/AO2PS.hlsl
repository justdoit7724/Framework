#include "ShaderInfo.cginc"

#define AO_INTENSITY_START_DISTZ 10.0f
#define AO_INTENSITY_MAX_DISTZ 2.0f

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 vFarPlane : TEXCOORD0;
    float2 tex : TEXCOORD1;
};

cbuffer CB_AO : register(b0)
{
    float4 offsetVector[14];
    float checkRad;
    float3 padding;
}
cbuffer CB_PROJ_UV_TRANSFORM : register(b1)
{
    float4x4 projUvMat;
}

Texture2D AOMap : register(t0);
SamplerState samp : register(s0);

float AOInTensity(float distZ)
{
    float intensity = 0;
    if(distZ > EPSILON)
    {
        float intensityLength = AO_INTENSITY_START_DISTZ - AO_INTENSITY_MAX_DISTZ;
        intensity = saturate((AO_INTENSITY_START_DISTZ - distZ) / intensityLength);
    }
    return intensity;
}

float4 main(PS_INPUT input) :SV_Target
{
    float4 sample = AOMap.SampleLevel(samp, input.tex, 0);
    float3 vNormal = sample.xyz;
    float vDepth = sample.w;
    
    //debug try pass through from vertex shader
    float3 vP = (vDepth / input.vFarPlane.z) * input.vFarPlane;

    //debug only check perpendicular point for now
    float occlusionSum = 0;
    
    float3 vQ = vP + vNormal * checkRad;
    float4 hPVQ = mul(projUvMat, float4(vQ, 1));
    float4 vRSample = AOMap.SampleLevel(samp, hPVQ.xy / hPVQ.w, 0);
    float3 vRNormal = vRSample.xyz;
    float vRDepth = vRSample.w;
    float3 vR = (vRDepth / vQ.z) * vQ;
    
    float distZ = vP.z - vR.z;
    float dp = saturate(dot(vNormal, normalize(vR - vP)));
    occlusionSum = dp*AOInTensity(distZ);

    return float4(occlusionSum.xxx, 1);

}