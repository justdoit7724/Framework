TextureCube cubeMap;
sampler samp;

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 lPos : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_Target
{
    return cubeMap.Sample(samp, input.lPos);
}