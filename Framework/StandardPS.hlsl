
#include "ShaderInfo.cginc"
#include "ShaderLight.cginc"

cbuffer EYE : register(b3)
{
    float4 eyePos;
};

cbuffer CB_TIME : register(b5)
{
    float elapsed;
}

TextureCube cmTex : register(t0);
Texture2D diffuseTex : register(t1);
Texture2D normalTex : register(t2);
Texture2D ssaoTex : register(t3);
Texture2D metalicTex : register(t4);
Texture2D roughnessTex : register(t5);
//...

SamplerState cmSamp : register(s0);
SamplerState samp : register(s1);

float3 GetBodyNormal(float2 tex)
{
    float3 ori_tex = normalTex.Sample(samp, tex).xyz;
    return (ori_tex * 2 - 1);
}
float3 ComputeMetalic(float3 color, float3 normal, float3 look, float2 uv)
{
    //debug modify metalic to mReflection
    float3 cm = cmTex.Sample(cmSamp, reflect(look, normal)).xyz;
    float metalic = metalicTex.SampleLevel(samp, uv, 0).x;
    return Lerp(color, cm, metalic);
}

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
    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent - dot(input.normal, input.tangent)*input.normal);
    float3 bitangent = cross(input.normal, input.tangent);
    float3x3 tbn = float3x3(input.tangent, bitangent, input.normal);
    float3 tNormal = GetBodyNormal(input.tex);
    float3 wNormal = normalize(mul(tNormal, tbn));

    float3 toEye = normalize(eyePos.xyz - input.wPos);
    float roughness = roughnessTex.SampleLevel(samp, input.tex, 0).x;
    
    float4 ambient = 0;
    float4 diffuse = 0;
    float4 specular = 0;
    float4 reflection = 0;
    float4 A, D, S;
    ComputeDirectionalLight(wNormal, toEye, roughness, A, D, S);
    ambient += A;
    diffuse += D;
    specular += S;

    ComputePointLight(input.wPos, wNormal, toEye, roughness, A, D, S);
    ambient += A;
    diffuse += D;
    specular += S;
    
    ComputeSpotLight(input.wPos, wNormal, toEye, roughness, A, D, S);
    ambient += A;
    diffuse += D;
    specular += S;
    
    float3 tex = diffuseTex.Sample(samp, input.tex).xyz;
    tex = ComputeMetalic(tex, wNormal, -toEye, input.tex);
    float4x4 uvMat = float4x4(
        0.5, 0, 0, 0,
        0, -0.5, 0, 0,
        0, 0, 0, 0,
        0.5, 0.5, 0, 1);
    input.pPos = mul(input.pPos, uvMat);
    float2 viewUV = input.pPos.xy / input.pPos.w;
    float ssao = ssaoTex.SampleLevel(samp, viewUV, 0).r;
    
    diffuse *= float4(tex, 1);
    ambient *= float4(tex, 1) * ssao;
    
    float4 color = ambient + diffuse + specular;
    color.w = mDiffuse.a;
    
    return color;
}