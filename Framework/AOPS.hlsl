struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float vDepth : TEXCOORD0;
    float3 vNormal : TEXCOORD1;
};

float4 main(PS_INPUT input):SV_Target
{
    return float4(normalize(input.vNormal), input.vDepth);
}