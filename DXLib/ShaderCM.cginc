#ifndef _SHADER_CM
#define _SHADER_CM

#include "ShaderInfo.cginc"
#include "ShaderReg.cginc"
#include "ShaderSampPoint.cginc"

#define REFRACTION_INDEX_GLASS 1.2

TextureCube cmTex : SHADER_REG_SRV_CM;

float3 ComputeTransparency(float3 wPos, float3 normal, float3 look)
{
    float3 rtex3d = Lerp(look, normalize(look - normal), saturate(REFRACTION_INDEX_GLASS - 1.01f));
    float3 gtex3d = Lerp(look, normalize(look - normal), saturate(REFRACTION_INDEX_GLASS - 1));
    float3 btex3d = Lerp(look, normalize(look - normal), saturate(REFRACTION_INDEX_GLASS - 0.99));
    return float3(
        cmTex.SampleLevel(pointSamp, rtex3d, 0).r,
        cmTex.SampleLevel(pointSamp, gtex3d, 0).g,
        cmTex.SampleLevel(pointSamp, btex3d, 0).b);
}
float3 ComputeReflect(float3 wNormal, float3 look)
{
    return cmTex.SampleLevel(pointSamp, reflect(look, wNormal), 0).xyz;
}

#endif