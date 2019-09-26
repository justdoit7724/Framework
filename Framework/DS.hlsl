
cbuffer CB_TRANSFORM : register(b0)
{
    float4x4 wvp_mat;
};

struct Patch
{
    float edgeTessFactor[3] : SV_TessFactor;
    float inTessFactor : SV_InsideTessFactor;
};

struct DS_INPUT
{
    float3 pos : TEXCOORD0;
};
struct DS_OUTPUT
{
    float4 pos : SV_POSITION;
};

[domain("tri")]
DS_OUTPUT main(Patch p, float2 uv : SV_DomainLocation, const OutputPatch<DS_INPUT, 3> input)
{
    DS_OUTPUT output;
    
    float3 right = input[1].pos - input[0].pos;
    float3 down = input[2].pos - input[0].pos;
    float3 p3 = input[0].pos+right * uv.x + down * uv.y;

    output.pos = mul(wvp_mat, float4(p3, 1));
    return output;
}