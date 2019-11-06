
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

//debug - modify texture system
TextureCube cm_tex : register(t0);
Texture2DArray bodyTex : register(t1);
Texture2DArray bodyNTex : register(t2);
Texture2D shadowTex : register(t3);
TextureCube pointLightShadowTex : register(t4);

SamplerState bodySampleState : register(s0);
SamplerState cmSampleState : register(s1);
SamplerComparisonState shadowSamp : register(s2);

float3 GetBodyNormal(float2 tex)
{
    float3 ori_tex = bodyNTex.Sample(bodySampleState, float3(tex, 0)).xyz;
    return (ori_tex * 2 - 1);
}
float3 ComputeMetalic(float3 color, float3 normal, float3 look)
{
    //debug modify metalic to mReflection
    float3 cm = cm_tex.Sample(cmSampleState, reflect(look, normal)).xyz;
    float metalic = 1.0f;
    return Lerp(color, cm, metalic);
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 tex : TEXCOORD2;
    float3 tangent : TEXCOORD3;
    float4 pt_ndc_pos : TEXCOORD4;
    float2 pMatZElem : TEXCOORD5;
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
    tex = ComputeMetalic(tex, wNormal, -toEye);
    float4 ambient = 0;
    float4 diffuse = 0;
    float4 specular = 0;
    float4 reflection = 0;
    float4 A, D, S;
    

    float shadowFactor = PointLightShadowFactor(input.normal, input.wPos, p_Pos[0].xyz, pointLightShadowTex, cmSampleState, input.pMatZElem);
    
    //debug set roughness
    ComputeDirectionalLight(wNormal, toEye,0, A, D, S);
    ambient += A;
    diffuse += D * shadowFactor;
    specular += S * shadowFactor;
    ComputePointLight(input.wPos, wNormal, toEye,0, A, D, S);
    ambient += A;
    diffuse += D * shadowFactor;
    specular += S * shadowFactor;
    ComputeSpotLight(input.wPos, wNormal, toEye,0, A, D, S);
    ambient += A;
    diffuse += D * shadowFactor;
    specular += S * shadowFactor;
    
    ambient *= float4(tex, 1);
    diffuse *= float4(tex, 1);
    
    float4 color = ambient + diffuse + specular;
    
    color.w = mDiffuse.a;
    return color;
}