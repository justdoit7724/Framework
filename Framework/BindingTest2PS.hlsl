struct PS_INPUT
{
    float4 pos : SV_POSITION;
};


float4 main(PS_INPUT input) : SV_Target
{
    return float4(1, 1, 1, 1);
}