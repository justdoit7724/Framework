#include "ShaderVertex.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 pPos : TEXCOORD0;
    float3 wPos : TEXCOORD1;
    float3 normal : TEXCOORD2;
    float2 tex : TEXCOORD3;
    float3 tangent : TEXCOORD4;
};

cbuffer CB_VS_PROPERTY : register(b0)
{
    float4x4 WMat;
    float4x4 VPMat;
    float4x4 NMat;
};

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 wPos = mul(WMat, float4(input.pos, 1));
    wPos.xyz /= wPos.w;
    output.wPos = wPos.xyz;
    output.pPos = mul(VPMat, float4(output.wPos, 1));
    output.pos = output.pPos;
    output.normal = mul((float3x3) NMat, input.normal);
    output.tex = input.tex;
    output.tangent = mul((float3x3) WMat, input.tangent);
    
    return output;
}