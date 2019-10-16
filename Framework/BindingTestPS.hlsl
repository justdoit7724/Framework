struct PS_INPUT
{
    float4 pos : SV_POSITION;
};

cbuffer CBps : register(b0)
{
    float4x4 wvpMat;
}
cbuffer CBps2 : register(b1)
{
    float4 pos;
}

float4 main(PS_INPUT input) : SV_Target
{
    return float4(1,1,1, 1);
}