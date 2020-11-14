#include "ShaderVertex.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

cbuffer CB_VS_PROPERTY : register(b0)
{
    float4x4 WVPMat;
};

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(WVPMat, float4(input.pos, 1));
    output.tex = input.tex;

    return output;
}