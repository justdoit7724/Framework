struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

Texture2D aoMap : register(t0);
SamplerState samp : register(s0);

float4 main(PS_INPUT input) :SV_Target
{
    float zDepth = aoMap.SampleLevel(samp, input.tex, 0);

    float3 vP = ;
    float3 vQ;
}