
#include "ShaderInfo.cginc"
#include "ShaderReg.cginc"
#include "ShaderLight.cginc"
#include "ShaderNormal.cginc"
#include "ShaderSampPoint.cginc"


cbuffer EYE : SHADER_REG_CB_EYE
{
    float4 eyePos;
};


Texture2D diffuseTex : SHADER_REG_SRV_DIFFUSE;

Texture2D<float2> texPrevDepth : register(t10);

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD1;
    float4 pPos : TEXCOORD2;
    float3 normal : TEXCOORD3;
    float2 tex : TEXCOORD4;
};

struct PS_OUTPUT
{
    float4 color : SV_Target;
    float depth : SV_Depth;
};

PS_OUTPUT main(PS_INPUT input)
{
    // depth check
    const float2 winSize = float2(600, 600);
    float2 tex = input.pPos.xy / input.pPos.w;
    tex = (tex + 1)*0.5;
    tex.y = 1 - tex.y;
    int2 mTex = int2(
        min(winSize.x - 1, tex.x * winSize.x),
        min(winSize.y - 1, tex.y * winSize.y));
    float newDepth = input.pPos.z / input.pPos.w;
    float curDepth = texPrevDepth.Load(int3(mTex, 0), 0).x;
    clip(newDepth - curDepth - EPSILON);
    
    // rendering calculation
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
    
    float3 surfaceTex = diffuseTex.Sample(pointSamp, input.tex).xyz;
    
    float3 ambient = (dAmbient + pAmbient + sAmbient) * surfaceTex;
    float3 diffuse = (dDiffuse + pDiffuse + sDiffuse) * surfaceTex;
    float3 spec = (dSpecular + pSpecular + sSpecular);
    
    PS_OUTPUT output;
    output.color = float4(ambient+diffuse+spec, mDiffuse.w);
    output.depth = newDepth;
    return output;

}