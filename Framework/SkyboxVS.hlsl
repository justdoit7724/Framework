#include "ShaderVertex.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 lPos : TEXCOORD0;
};

cbuffer CB_VS_PROPERTY : register(b0)
{
    float4x4 w_mat;
};

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.lPos = input.pos;
    output.pos = mul(w_mat, float4(input.pos, 1));
    
    return output;
}