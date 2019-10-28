struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 farPlanePos : TEXCOORD0;
    float2 tex : TEXCOORD1;
};

Texture2D AOMap : register(t0);
SamplerState samp : register(s0);

float4 main(PS_INPUT input) :SV_Target
{
    float4 sample = AOMap.Sample(samp, input.tex);
    float3 vNormal = sample.xyz;
    float vDepth = sample.w;

}