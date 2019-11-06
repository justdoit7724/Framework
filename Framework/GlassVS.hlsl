#include "ShaderVertex.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 wNormal : TEXCOORD0;
    float3 wPos : TEXCOORD1;
};

cbuffer CB_TRANSFORM : register(b0)
{
    float4x4 wMat;
    float4x4 vpMat;
    float4x4 nMat;
}

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;

    output.wPos = mul(wMat, float4(input.pos, 1)).xyz;
    output.pos = mul(vpMat, float4(output.wPos, 1));
    output.wNormal = mul((float3x3) nMat, input.normal);
    return output;
}