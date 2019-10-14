
#include "ShaderInfo.cginc"
#include "ShaderLight.cginc"
#include "ShaderShadow.cginc"

cbuffer EYE : register(b3)
{
    float4 eyePos;
};


cbuffer CB_TIME : register(b5)
{
    float elapsed;
}

TextureCube cm_tex : register(t0);
Texture2DArray bodyTex : register(t1);
Texture2DArray bodyNTex : register(t2);
Texture2D shadowTex : register(t3);

SamplerState bodySampleState : register(s0);
SamplerState cmSampleState : register(s1);
SamplerComparisonState shadowSamp : register(s2);

float3 GetBodyNormal(float2 tex)
{
    float3 ori_tex = bodyNTex.Sample(bodySampleState, float3(tex, 0)).xyz;
    return (ori_tex * 2 - 1);
}
void ComputeReflection(float3 normal, float3 look, out float4 color)
{
    color = cm_tex.Sample(cmSampleState, reflect(look, normal)) * mReflection;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 tex : TEXCOORD2;
    float3 tangent : TEXCOORD3;
    float4 pt_ndc_pos : TEXCOORD4;
};
float4 main(PS_INPUT input) : SV_Target
{
    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent - dot(input.normal, input.tangent) * input.normal);
    float3 bitangent = cross(input.normal, input.tangent);
    float3x3 tbn = float3x3(input.tangent, bitangent, input.normal);
    
    float3 tNormal = GetBodyNormal(input.tex);
    
    float3 wNormal = normalize(mul(tNormal, tbn));
    
    float3 toEye = normalize(eyePos.xyz - input.wPos);
    float3 tex = bodyTex.Sample(bodySampleState, float3(input.tex, 0)).xyz;
    float4 ambient = 0;
    float4 diffuse = 0;
    float4 specular = 0;
    float4 reflection = 0;
    float4 A, D, S;
    float shadowFactor = CalcShadowFactor(shadowSamp, shadowTex, input.pt_ndc_pos, 760 * 3, 760 * 3);

    ComputeDirectionalLight(wNormal, toEye, A, D, S);
    ambient += A;
    diffuse += D * shadowFactor;
    specular += S * shadowFactor;
    ComputePointLight(input.wPos, wNormal, toEye, A, D, S);
    ambient += A;
    diffuse += D * shadowFactor;
    specular += S * shadowFactor;
    ComputeSpotLight(input.wPos, wNormal, toEye, A, D, S);
    ambient += A;
    diffuse += D * shadowFactor;
    specular += S * shadowFactor;
    ComputeReflection(wNormal, -toEye, reflection);
    
    ambient *= float4(tex, 1);
    diffuse *= float4(tex, 1);
    
    float4 color = ambient + diffuse + specular;
    color = Lerp(color, reflection, mReflection.w);
    
    //debug - remove
    color = Lerp(color, float4(0, 0, 0, 0), 1 - shadowFactor);
        
    
    color.w = mDiffuse.a;
    return color;
}