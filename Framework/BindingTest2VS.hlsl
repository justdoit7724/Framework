#include "ShaderVertex.cginc"
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
};

cbuffer CB_VS2 : register(b0)
{
    float4 pos;
}

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = pos;
    return output;
}