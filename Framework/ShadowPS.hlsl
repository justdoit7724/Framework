
cbuffer CB_TRANSPARENCY : register(b0)
{
    float transparency;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
};

float4 main(PS_INPUT input) : SV_Target
{
    return float4(0, 0, 0, transparency);

}
