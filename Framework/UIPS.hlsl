#define MAGENTA float3(1,0,1)
#define TRANSPARANCY float4(0,0,0,0)

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2DArray uiTexture : register(t0);
SamplerState sampState : register(s0);

cbuffer CB_SLICE_INDEX : register(b0)
{
    float sliceIdx;
};

float4 main(PS_INPUT input) : SV_Target
{
    float4 color = uiTexture.Sample(sampState, float3(input.tex, sliceIdx));
    
    //delete
    color.w = 0.25f;
    return color;

    if(color.x == MAGENTA.x && 
        color.y == MAGENTA.y &&
        color.z == MAGENTA.z)
    {
        clip(-1.0f);
        return 0;
    }
    else
    {
        return color;
    }
}