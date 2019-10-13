struct PS_INPUT
{
    float4 pos : SV_POSITION;
};

cbuffer CB_PS : register(b0)
{
    float4 pos;
}

float4 main(PS_INPUT input) : SV_Target
{
    return float4(pos.xyz, 1);
}