

#include "ShaderReg.cginc"
#include "ShaderSampPoint.cginc"

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

cbuffer CB_TRANSP : register(b0)
{
    float transp;
}

Texture2D uiTexture : SHADER_REG_SRV_DIFFUSE;

float4 main(PS_INPUT input) : SV_Target
{
    float4 color = uiTexture.SampleLevel(pointSamp, input.tex, 0);
    color.w *= transp;
    
    return color;
}