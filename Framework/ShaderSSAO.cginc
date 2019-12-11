#ifndef _SHADER_SSAO
#define _SHADER_SSAO

#include "ShaderReg.cginc"

Texture2D ssaoTex : SHADER_REG_SRV_SSAO;

float ComputeSSAO(SamplerState samp, float4 pPos)
{
    float2 viewUV = pPos.xy / pPos.w;
    viewUV.x = viewUV.x * 0.5f + 0.5f;
    viewUV.y = -viewUV.y * 0.5f + 0.5f;
    float ssao = ssaoTex.SampleLevel(samp, viewUV, 0).r;
    return pow(ssao, 1.8f);
}

#endif