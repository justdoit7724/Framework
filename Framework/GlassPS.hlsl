struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 wNormal : TEXCOORD0;
    float3 wPos : TEXCOORD1;
};

cbuffer CB_EYE : register(b0)
{
    float4 eye;
}

TextureCube cm : register(t0);
SamplerState samp : register(s0);

float4 main(PS_INPUT input) : SV_Target
{
    input.wNormal = normalize(input.wNormal);
    float3 lookDir = normalize(input.wPos - eye.xyz);
    float3 tex2d = refract(lookDir, input.wNormal, 1.33f);

    return float4(cm.SampleLevel(samp, tex2d, 0).xyz, 1);

}