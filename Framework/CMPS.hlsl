#include "ShaderReg.cginc"

TextureCube cubeMap : SHADER_REG_PS_SRV_CM;

sampler samp : SHADER_REG_PS_SAMP_CM;

cbuffer CB_EYE : SHADER_REG_PS_CB_EYE
{
    float4 eye;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_Target
{
    float3 wDir = normalize(input.wPos - eye.xyz);
    wDir = reflect(wDir, normalize(input.normal));
    
    return cubeMap.Sample(samp, wDir);
}