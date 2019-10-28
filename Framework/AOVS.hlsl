#include "ShaderInfo.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 vNormal : TEXCOORD0;
    float3 vPos : TEXCOORD1;
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
    float3 wPos = mul(worldMat, float4(input.pos, 1)).xyz;
    output.vPos = mul(viewMat, float4(wPos, 1)).xyz;
    output.pos = mul(projMat, float4(output.vPos, 1));
    float3 wNormal = mul((float3x3) normalMat, input.normal);
    output.vNormal = mul((float3x3) viewMat, wNormal);
    return output;
}