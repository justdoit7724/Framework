TextureCube cubeMap : register(t0);

sampler samp : register(s0);

cbuffer CB_EYE : register(b0)
{
    float4 eye;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_Target
{
    float3 wDir = normalize(input.wPos - eye.xyz);
    wDir = reflect(wDir, normalize(input.normal));
    
    return cubeMap.Sample(samp, wDir);
}