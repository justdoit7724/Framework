cbuffer CB_GS : register(b0)
{
    float4 pos;
}


struct GS_INPUT
{
    float3 pos : TEXCOORD0;
};
struct GS_OUTPUT
{
    float4 pos : SV_POSITION;
};

[maxvertexcount(3)]
void main(triangle GS_INPUT input[3], inout TriangleStream<GS_OUTPUT> stream)
{
    GS_OUTPUT output[3];
    output[0].pos = float4(input[0].pos, 1);
    output[1].pos = float4(input[1].pos, 1);
    output[2].pos = float4(input[2].pos, 1);

    stream.Append(output[0]);
    stream.Append(output[1]);
    stream.Append(output[2]);
}