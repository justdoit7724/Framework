#include "ShaderVertex.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

cbuffer CB_VS_PROPERTY : register(b0)
{
    float4x4 WMat;
    float4x4 VMat;
    float4x4 PMat;
    float4x4 NMat;
    
    //not use
    float near;
    float far;
    float aspect;
    float scnRatio;
};

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    float4 worldPos = mul(WMat, float4(input.pos, 1));
    float4 vPos = mul(VMat, worldPos);
    float4 pPos = mul(PMat, vPos);
    output.pos = pPos;
    output.tex = input.tex;

    return output;
}