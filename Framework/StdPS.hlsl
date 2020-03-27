
#include "ShaderInfo.cginc"
#include "ShaderLight.cginc"
#include "ShaderReg.cginc"
#include "ShaderNormal.cginc"
#include "ShaderSampPoint.cginc"
#include "ShaderSampAniso.cginc"

cbuffer EYE : SHADER_REG_CB_EYE
{
    float4 eyePos;
};


Texture2D diffuseTex : SHADER_REG_SRV_DIFFUSE;
//...


struct PS_INPUT
{
    float4 pos : SV_POSITION;
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
    ComputeDirectionalLight(wNormal, -look, 0, ambient, diffuse, specular);
    
    
    float3 tex = diffuseTex.Sample(anisotropicSamp, input.tex).xyz;
    
    diffuse *= tex;
    ambient *= tex;
    
    return float4(specular + diffuse + ambient, 1);
}