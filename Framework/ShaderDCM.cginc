#ifndef _SHADER_DCM
#define _SHADER_DCM

#include "ShaderInfo.cginc"
#include "ShaderReg.cginc"
#include "ShaderSampPoint.cginc"
#include "ShaderSampLinearPoint.cginc"

#define REFRACTION_INDEX_GLASS 1.2

TextureCube cmTex : SHADER_REG_SRV_CM;
TextureCube dcmTex : SHADER_REG_SRV_DCM;


float3 ComputeTransparency(float3 wPos, float3 normal, float3 look)
{
    float3 rtex3d = Lerp(look, normalize(look - normal), saturate(REFRACTION_INDEX_GLASS - 1.01f));
    float3 gtex3d = Lerp(look, normalize(look - normal), saturate(REFRACTION_INDEX_GLASS - 1));
    float3 btex3d = Lerp(look, normalize(look - normal), saturate(REFRACTION_INDEX_GLASS - 0.99));
    
    float3 cm = float3(
    cmTex.SampleLevel(linearPointSamp, rtex3d, 0).r,
    cmTex.SampleLevel(linearPointSamp, gtex3d, 0).g,
    cmTex.SampleLevel(linearPointSamp, btex3d, 0).b);
    
    float4 dcmr = dcmTex.SampleLevel(linearPointSamp, rtex3d, 0);
    float4 dcmg = dcmTex.SampleLevel(linearPointSamp, gtex3d, 0);
    float4 dcmb = dcmTex.SampleLevel(linearPointSamp, btex3d, 0);
    
    return float3(
    lerp(cm.r, dcmr.r, dcmr.w),
    lerp(cm.g, dcmg.g, dcmg.w),
    lerp(cm.b, dcmb.b, dcmb.w));
}
float3 ComputeReflect(float3 wNormal, float3 look)
{
    float3 reflDir = reflect(look, wNormal);
    
    float3 cm = cmTex.SampleLevel(linearPointSamp, reflDir, 0).xyz;
    float4 dcm = dcmTex.SampleLevel(linearPointSamp, reflDir, 0);
    return Lerp(cm, dcm.xyz, dcm.w);
}

#endif