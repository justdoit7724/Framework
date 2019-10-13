#include "ShaderInfo.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
};

cbuffer CB_TRANSFORMATION : register(b0)
{
    float4x4 wvpMat;
};

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(wvpMat, float4(input.pos, 1));

    return output;
}