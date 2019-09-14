
cbuffer CB_WORLD_MATRIX : register(b0)
{
    float4x4 World_Mat;
    float4x4 VP_Mat;
};

struct VS_INPUT
{
    float3 pos : POSITION;
};
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 wPos = mul(World_Mat, float4(input.pos, 1));
    wPos /= wPos.w;
    output.pos = mul(VP_Mat, wPos);

    return output;
}
