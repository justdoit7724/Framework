
#include "ShaderVertex.cginc"

cbuffer CB_WORLD_MATRIX : register(b0)
{
    float4x4 wvp;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
};

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(wvp, float4(input.pos, 1));
    return output;
}
