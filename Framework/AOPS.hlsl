struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 vNormal : TEXCOORD0;
    float vDepth : TEXCOORD1;
};

float4 main(PS_INPUT input):SV_Target
{
    return float4(normalize(input.vNormal), input.vDepth);

}