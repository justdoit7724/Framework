
#include "ShaderInfo.cginc"
#include "ShaderLight.cginc"
#include "ShaderReg.cginc"
#include "ShaderNormal.cginc"


SHADER_REG_CB_EYE
SHADER_REG_SRV_MATERIAL
SHADER_REG_SAMP_POINT

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD1;
    float4 pPos : TEXCOORD2;
    float3 normal : TEXCOORD3;
    float2 tex : TEXCOORD4;
    //float3 tangent : TEXCOORD4;
};

float4 main(PS_INPUT input) : SV_Target
{
	input.normal = normalize(input.normal);

    float3 v = normalize(eyePos.xyz - input.wPos);
    
    float3 dAmbient = 0;
    float3 dDiffuse = 0;
    float3 dSpecular = 0;
    ComputeDirectionalLight(input.normal, v, dAmbient, dDiffuse, dSpecular);
    float3 pAmbient = 0;
    float3 pDiffuse = 0;
    float3 pSpecular = 0;
    ComputePointLight(input.wPos, input.normal, v, pAmbient, pDiffuse, pSpecular);
    float3 sAmbient = 0;
    float3 sDiffuse = 0;
    float3 sSpecular = 0;
    ComputeSpotLight(input.wPos, input.normal, v, sAmbient, sDiffuse, sSpecular);
    
    float3 tex = srvSubMatDiff.Sample(pointSamp, input.tex).xyz;
    
    float3 ambient = (dAmbient + pAmbient + sAmbient) * tex;
    float3 diffuse = (dDiffuse + pDiffuse + sDiffuse) * tex;
    float3 spec = (dSpecular + pSpecular + sSpecular);
    
    return float4(input.normal, mDiffuse.w);
}