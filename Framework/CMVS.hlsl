#include "ShaderVertex.cginc"

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
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
    
    output.wPos = mul(WMat, float4(input.pos, 1)).xyz;
    output.pos = mul(VPMat, float4(output.wPos, 1));
    output.normal = mul(NMat, float4(input.normal, 1)).xyz;
    
    return output;
}