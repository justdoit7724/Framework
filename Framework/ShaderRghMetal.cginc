#ifndef _SHADER_RGH_METAL
#define _SHADER_RGH_METAL

#include "ShaderReg.cginc"

Texture2D roughnessTex : SHADER_REG_SRV_ROUGHNESS;
Texture2D metallicTex : SHADER_REG_SRV_METALLIC;

float ComputeRoughness(SamplerState samp, float2 tex)
{
    return roughnessTex.Sample(samp, tex).r;
}
float ComputeMetallic(SamplerState samp, float2 tex)
{
    return metallicTex.Sample(samp, tex).r;
}

#endif