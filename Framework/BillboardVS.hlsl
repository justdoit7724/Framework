
struct VS_INPUT
{
    float3 pos : POSITION;
    float2 size : SIZE;
};
struct VS_OUTPUT
{
    float3 pos : TEXCOORD0;
    float2 size : TEXCOORD1;
};

cbuffer CB_PROPERTY : register(b0)
{
    float4x4 mat;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = input.pos;
    output.size = input.size;
    return output;
}