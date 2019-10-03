#include "ShaderInfo.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

cbuffer CB_VS_PROPERTY : register(b0)
{
    float4x4 WMat;
    float4x4 VPMat;
    float4x4 NMat;
    float4x4 texMat;
};

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    float4 worldPos = mul(WMat, float4(input.pos, 1));
    output.pos = mul(VPMat, worldPos);
    output.tex = mul(texMat, float4(input.tex.x, input.tex.y, 0, 1)).xy;

    return output;
}