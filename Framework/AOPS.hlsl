struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 vNormal : TEXCOORD0;
    float3 vPos : TEXCOORD1;
};

float4 main(PS_INPUT input)
{
    return float4(input.vNormal, input.vPos.z);
}