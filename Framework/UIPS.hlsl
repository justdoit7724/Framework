#define MAGENTA float3(1,0,1)
#define TRANSPARANCY float4(0,0,0,0)

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

cbuffer CB_TRANSP : register(b0)
{
    float transp;
}

Texture2D uiTexture : register(t0);
SamplerState sampState : register(s0);

float4 main(PS_INPUT input) : SV_Target
{
    float4 color = uiTexture.SampleLevel(sampState, input.tex, 0);
    color.w *= transp;
    
    return color;
}