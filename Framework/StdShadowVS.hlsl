#include "ShaderInfo.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 tex : TEXCOORD2;
    float3 tangent : TEXCOORD3;
    float4 pt_ndc_pos : TEXCOORD4;
};

cbuffer CB_TRANSFORMATION : register(b0)
{
    float4x4 WMat;
    float4x4 VPMat;
    float4x4 NMat;
    float4x4 uiMat;
    float4x4 pt_vpt_mat;
};

VS_OUTPUT main(STD_VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 wPos = mul(WMat, float4(input.pos, 1));
    output.wPos = wPos.xyz;
    output.pos = mul(VPMat, float4(output.wPos, 1));
    output.normal = mul((float3x3) NMat, input.normal);
    output.tex = mul(uiMat, float4(input.tex, 0, 1)).xy;
    output.tangent = mul((float3x3) WMat, input.tangent);
    output.pt_ndc_pos = mul(pt_vpt_mat, wPos);

    return output;
}