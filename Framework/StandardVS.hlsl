struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD;
    float3 normal : TEXCOORD2;
    float2 tex : TEXCOORD3;
};

cbuffer CB_VS_PROPERTY : register(b0)
{
    float4x4 WMat;
    float4x4 VPMat;
    float4x4 NMat;
    float4x4 texMat;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.wPos = mul((float3x3)WMat, input.pos);
    output.pos = mul(VPMat, float4(output.wPos, 1));
    output.normal = mul((float3x3) NMat, input.normal);
    output.tex = mul(texMat, float4(input.tex, 0, 1)).xy;
    
    return output;
}