
#include "ShaderReg.cginc"
#include "ShaderSampPoint.cginc"

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D uiTexture : SHADER_REG_SRV_DIFFUSE;

float4 main(PS_INPUT input) : SV_Target
{
    float3 color = uiTexture.SampleLevel(pointSamp, input.tex, 0).xyz;
    
    return float4(color, 1);
}