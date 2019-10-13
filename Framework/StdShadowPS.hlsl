
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
    
    ambient *= float4(tex, 1);
    diffuse *= float4(tex, 1);
    
    float4 color = ambient + diffuse + specular;
    color = Lerp(color, reflection, mReflection.w);
    color.w = mDiffuse.a;
    
    float3x3 ptUIMat = float3x3(
        0.5, 0, 0,
        0, -0.5, 0,
        0.5, 0.5, 1);
    float oriDepth = input.pt_ndc_pos.z;
    input.pt_ndc_pos /= input.pt_ndc_pos.w;
    float depth = input.pt_ndc_pos.z;
    float2 pt_ui = mul(float3(input.pt_ndc_pos.xy, 1), ptUIMat).xy;

    //debug - remove
    const float SMAP_WIDTH = 760;
    const float SMAP_HEIGHT = 760;
    const float2 SMAP_DX = float2(1.0f / SMAP_WIDTH, 0);
    const float2 SMAP_DY = float2(0, 1.0f / SMAP_HEIGHT);
    float shadowDepth = shadowTex.SampleCmpLevelZero(shadowSamp, pt_ui, depth).x;
    if (oriDepth > 0 && pt_ui.x == saturate(pt_ui.x) && pt_ui.y == saturate(pt_ui.y))
    {
        return float4(shadowDepth.xxx, 1);
        //return lerp(
        //    lerp(r, rR, t.x),
        //    lerp(rB, rBR, t.x),
        //    t.y);
    }
    else
    {
        return float4(0,0,0, 1);
    }
    
    return color;
}