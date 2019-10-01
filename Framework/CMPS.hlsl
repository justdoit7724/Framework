TextureCube cubeMap : register(t0);

sampler samp : register(s0);

cbuffer CB_EYE : register(b0)
{
    float3 eye;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_Target
{
    float3 wDir = normalize(input.wPos - eye);
    wDir = reflect(wDir, normalize(input.normal));
    
    //float3 mDir = refract(dir, input.normal, 1.0f);

    return cubeMap.Sample(samp, wDir);
}