struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 wNormal : TEXCOORD0;
    float3 wPos : TEXCOORD1;
};

cbuffer CB_EYE : register(b3)
{
    float4 eye;
}

TextureCube cm : register(t0);
SamplerState samp : register(s0);

float4 main(PS_INPUT input) : SV_Target
{
    input.wNormal = normalize(input.wNormal);
    float3 lookDir = normalize(input.wPos - eye.xyz);
    float3 rtex2d = refract(lookDir, input.wNormal, 2.0f);
    float3 gtex2d = refract(lookDir, input.wNormal, 2.0f);
    float3 btex2d = refract(lookDir, input.wNormal, 2.0f);

    return float4(
    cm.SampleLevel(samp, rtex2d, 0).x,
    cm.SampleLevel(samp, gtex2d, 0).y,
    cm.SampleLevel(samp, btex2d, 0).z, 1);
}