struct VS_INPUT
{
    float3 pos : POSITION;
};
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 pPos : TEXCOORD1;
};

cbuffer CB_VS_PROPERTY : register(b0)
{
    float4x4 wvp_mat;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(wvp_mat, float4(input.pos, 1));
    output.pPos = output.pos;

    return output;
}