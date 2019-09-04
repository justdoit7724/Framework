
cbuffer CB_VS_PROPERTY : register(b0)
{
    float4x4 WMat;
    float4x4 VPMat;
    float4x4 NMat;
    float renderPriority;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    
    float4x4 wvp = mul(VPMat, WMat);
    output.pos = mul(wvp, float4(input.pos, 1));
    output.normal = mul(NMat, float4(input.normal, 1)).xyz;
    output.tex = input.tex;

    return output;
}