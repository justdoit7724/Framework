#include "ShaderVertex.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float3 wNormal : TEXCOORD0;
    float pDist : TEXCOORD1;
};

cbuffer CB_TRANSFORMATION : register(b0)
{
    float4x4 wvpMat;
    float4x4 normalMat;
}

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(wvpMat, float4(input.pos, 1));
    output.pDist = output.pos.z / output.pos.w;
    output.wNormal = mul((float3x3)normalMat, input.normal);
    return output;
}