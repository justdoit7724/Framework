#include "ShaderReg.cginc"

cbuffer CB_COLOR : SHADER_REG_CB_COLOR
{
    float4 color;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
};

float4 main(PS_INPUT input) : SV_Target
{
    return color;
}