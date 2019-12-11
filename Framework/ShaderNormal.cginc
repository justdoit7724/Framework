
#ifndef _SHADER_NORMAL
#define _SHADER_NORMAL

#include "ShaderReg.cginc"

Texture2D normalTex : SHADER_REG_SRV_NORMAL;

float3 GetBodyNormal(SamplerState samp, float2 tex, float3 vNormal, float3 vTangent)
{
    vNormal = normalize(vNormal);
    vTangent = normalize(vTangent);
    vTangent = normalize(vTangent - dot(vNormal, vTangent) * vNormal);
    float3 vBitangent = cross(vNormal, vTangent);
    float3x3 tbn = float3x3(vTangent, vBitangent, vNormal);
    
    float3 texNormal = normalTex.Sample(samp, tex).xyz;
    texNormal = normalize(texNormal * 2 - 1);
    
    return mul(texNormal, tbn);
}

#endif