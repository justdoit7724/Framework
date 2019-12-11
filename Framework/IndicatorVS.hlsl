#include "ShaderVertex.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 ui : TEXCOORD0;
};

cbuffer CB_VS_PROPERTY : register(b0)
{
    float4x4 WVP;
};

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.pos = mul(WVP, float4(input.pos, 1));
    output.ui = input.tex;
    
    return output;
}