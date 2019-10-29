#include "ShaderVertex.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 vNormal : TEXCOORD0;
    float vDepth : TEXCOORD1;
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
    float4 wPos = mul(worldMat, float4(input.pos, 1));
    float4 vPos = mul(viewMat, wPos);
    output.vDepth = vPos.z;
    output.pos = mul(projMat, vPos);
    float3 wNormal = mul((float3x3) worldMat, input.normal);
    output.vNormal = mul((float3x3) viewMat, wNormal);
    return output;
}