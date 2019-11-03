#include "ShaderVertex.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float vDepth : TEXCOORD0;
    float3 vNormal : TEXCOORD1;
};

cbuffer CB_TRANSF_AO : register(b0)
{
    float4x4 worldMat;
    float4x4 viewMat;
    float4x4 projMat;
    float4x4 normalMat;
}

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    float4 worldPos = mul(worldMat, float4(input.pos, 1));
    float4 viewPos = mul(viewMat, worldPos);
    output.vDepth = viewPos.z;
    output.pos = mul(projMat, viewPos);
    float3 wNormal = mul((float3x3) normalMat, input.normal);
    output.vNormal = mul((float3x3) viewMat, wNormal);
    return output;
}