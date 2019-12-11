cbuffer CB_COLOR : register(b0)
{
    float4 color;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
};

float4 main(PS_INPUT input) : SV_Target
{
    return color;
}