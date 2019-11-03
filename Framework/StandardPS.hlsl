
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
//...

SamplerState cmSamp : register(s0);
SamplerState samp : register(s1);

float3 GetBodyNormal(float2 tex)
{
    float3 ori_tex = normalTex.Sample(samp, tex).xyz;
    return (ori_tex * 2 - 1);
}
void ComputeReflection(float3 normal, float3 look, out float4 color)
{
    color = cmTex.Sample(cmSamp, reflect(look, normal)) * mReflection;
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
    
    float4 ambient = 0;
    float4 diffuse = 0;
    float4 specular = 0;
    float4 reflection = 0;
    float4 A, D, S;
    ComputeDirectionalLight(wNormal, toEye, A, D, S);
    ambient += A;
    diffuse += D;
    specular += S;

    ComputePointLight(input.wPos, wNormal, toEye, A, D, S);
    ambient += A;
    diffuse += D;
    specular += S;
    
    ComputeSpotLight(input.wPos, wNormal, toEye, A, D, S);
    ambient += A;
    diffuse += D;
    specular += S;
    ComputeReflection(wNormal, -toEye, reflection);
    
    float3 tex = diffuseTex.Sample(samp, input.tex).xyz;
    diffuse *= float4(tex, 1);

    
    float4x4 uvMat = float4x4(
        0.5, 0, 0, 0,
        0, -0.5, 0, 0,
        0, 0, 0, 0,
        0.5, 0.5, 0, 1);
    input.pPos = mul(input.pPos, uvMat);
    float2 viewUV = input.pPos.xy / input.pPos.w;
    float ssao = ssaoTex.SampleLevel(samp, viewUV, 0).r;

    ambient *= float4(tex, 1) * ssao;
    
    float4 color = ambient + diffuse + specular;
    color = Lerp(color, reflection, mReflection.w);
    color.w = mDiffuse.a;
    
    return color;
}