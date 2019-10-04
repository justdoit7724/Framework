#include "ShaderInfo.cginc"

#define TESS_FACTOR_DIST_MAX 700
#define TESS_FACTOR_DIST_MIN 100
#define TESS_FACTOR_MAX 10
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
    float4x4 texMat;
};
cbuffer CB_EYE : register(b1)
{
    float3 eye;
};


VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.wPos = mul(WMat, float4(input.pos, 1)).xyz;
    output.pos = mul(VPMat, float4(output.wPos, 1));
    output.normal = mul((float3x3) NMat, input.normal);
    output.tex = mul(texMat, float4(input.tex, 0, 1)).xy;
    output.tangent = mul((float3x3) WMat, input.tangent);
    
    float sDist = dot(eye, output.wPos);
    float t = saturate((TESS_FACTOR_DIST_MAX - sDist) /(TESS_FACTOR_DIST_MAX - TESS_FACTOR_DIST_MIN));
    output.tessFactor = TESS_FACTOR_MIN + t * (TESS_FACTOR_MAX - TESS_FACTOR_MIN);

    return output;
}