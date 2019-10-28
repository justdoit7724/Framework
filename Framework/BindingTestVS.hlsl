#include "ShaderVertex.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
};

cbuffer CB1 : register(b0)
{
    float4x4 wvpMat;
}
cbuffer CB2 : register(b1)
{
    float4 pos;
}

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(wvpMat, float4(input.pos,1));
    return output;
}