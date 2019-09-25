struct VS_INPUT
{
    float3 pos : POSITION;
};

struct VS_OUTPUT
{
    float3 pos : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = input.pos;
    return output;
}