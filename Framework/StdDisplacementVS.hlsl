#include "ShaderVertex.cginc"

#define TESS_FACTOR_DIST_MAX 600
#define TESS_FACTOR_DIST_MIN 50
#define TESS_FACTOR_MAX 64
#define TESS_FACTOR_MIN 1

struct VS_OUTPUT
{
    float3 wPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 tex : TEXCOORD2;
    float3 tangent : TEXCOORD3;
    float tessFactor : TEXCOORD4;
};

cbuffer CB_VS_PROPERTY : register(b0)
{
    float4x4 WMat;
    float4x4 VPMat;
    float4x4 NMat;
};
cbuffer CB_EYE : register(b1)
{
    float4 eye;
};


VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.wPos = mul(WMat, float4(input.pos, 1)).xyz;
    output.normal = mul((float3x3) NMat, input.normal);
    output.tex = input.tex;
    output.tangent = mul((float3x3) WMat, input.tangent);
    
    float dist = length(eye.xyz - output.wPos);
    float t = saturate((TESS_FACTOR_DIST_MAX - dist) /(TESS_FACTOR_DIST_MAX - TESS_FACTOR_DIST_MIN));
    output.tessFactor = TESS_FACTOR_MIN + t * (TESS_FACTOR_MAX - TESS_FACTOR_MIN);
  
    return output;
}