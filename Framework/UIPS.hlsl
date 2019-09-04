#define MAGENTA float3(1,0,1)
#define TRANSPARANCY float4(0,0,0,0)

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D<float4> uiTexture : register(t0);
SamplerState sampState : register(s0);



float4 main(PS_INPUT input) : SV_Target
{
    float4 color = uiTexture.Sample(sampState, input.tex);

    if(color.x == MAGENTA.x && 
        color.y == MAGENTA.y &&
        color.z == MAGENTA.z)
    {
        return TRANSPARANCY;
    }
    else
    {
        return color;
    }
}