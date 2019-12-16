
#include "ShaderInfo.cginc"
#include "ShaderLight.cginc"
#include "ShaderReg.cginc"
#include "ShaderCM.cginc"
#include "ShaderNormal.cginc"
#include "ShaderSampPoint.cginc"
#include "ShaderSampAniso.cginc"
#include "ShaderRghMetal.cginc"

cbuffer EYE : SHADER_REG_CB_EYE
{
    float4 eyePos;
};


Texture2D diffuseTex : SHADER_REG_SRV_DIFFUSE;
//...


struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 pPos : TEXCOORD0;
    float3 wPos : TEXCOORD1;
    float3 normal : TEXCOORD2;
    float2 tex : TEXCOORD3;
    float3 tangent : TEXCOORD4;
};
float4 main(PS_INPUT input) : SV_Target
{
    float3 wNormal = GetBodyNormal(anisotropicSamp, input.tex, input.normal, input.tangent);

    float3 look = normalize(input.wPos - eyePos.xyz);
    
    float3 ambient = 0;
    float3 diffuse = 0;
    float3 specular = 0;
    float roughness = ComputeRoughness(anisotropicSamp, input.tex);
    float metallic = ComputeMetallic(anisotropicSamp, input.tex);
    ComputeDirectionalLight(wNormal, -look, roughness, ambient, diffuse, specular);
 
    specular *= (1 - roughness);
    float transpT = mDiffuse.a * (1 - roughness);
    float reflecT = metallic * (1 - roughness);
    
    float3 transp = ComputeTransparency(input.wPos, wNormal, look);
    float3 tex = diffuseTex.Sample(anisotropicSamp, input.tex).xyz;
    float3 reflec = ComputeReflect(wNormal, look);
    
    diffuse *= tex;
    ambient *= tex;
    
    float3 surface = Lerp(ambient + diffuse, transp, transpT);
    surface = Lerp(surface, reflec, reflecT);
    
    
    return float4(specular + surface, 1);
}