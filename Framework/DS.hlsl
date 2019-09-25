
cbuffer CB_TRANSFORM : register(b0)
{
    float4x4 wvp_mat;
};

struct Patch
{
    float edgeTessFactor[4] : SV_TessFactor;
    float inTessFactor[2] : SV_InsideTessFactor;
};

struct DS_INPUT
{
    float3 pos : TEXCOORD0;
};
struct DS_OUTPUT
{
    float4 pos : SV_POSITION;
};

[domain("quad")]
DS_OUTPUT main(Patch p, float2 uv : SV_DomainLocation, const OutputPatch<DS_INPUT, 4> input)
{
    DS_OUTPUT output;

    float3 p1 = lerp(input[0].pos, input[1].pos, 1 - uv.y);
    float3 p2 = lerp(input[3].pos, input[2].pos, 1 - uv.y);
    float3 p3 = lerp(p1, p2, uv.x);
    //p3.y = 0.3f * (p3.z * sin(p3.x) + p3.x * cos(p3.z));

    output.pos = mul(wvp_mat, float4(p3, 1));
    return output;
}