
#include "ShaderReg.cginc"
#include "ShaderSampPoint.cginc"

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

cbuffer CB_COLOR : register(b6)
{
    float4 mulColor;
};

Texture2DMS<float2> uiTexture : SHADER_REG_SRV_DIFFUSE;
//Texture2D uiTexture : SHADER_REG_SRV_DIFFUSE;

float4 main(PS_INPUT input) : SV_Target
{
    
    int2 index = int2(input.tex.x * 600, input.tex.y * 600);
    
    float3 color = uiTexture.Load(index, 0).xxx;
    color *= mulColor;
    return float4(color, 1);
    
    //float3 color = uiTexture.SampleLevel(pointSamp, input.tex, 0).xyz;
    //color *= mulColor;
    //return float4(color, 1);
}