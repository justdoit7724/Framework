
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 pPos : TEXCOORD1;
};

Texture2DArray uiTexture : register(t0);
SamplerState sampState : register(s0);

cbuffer CB_SLICE_INDEX : register(b0)
{
    float sliceIdx;
};

float4 main(PS_INPUT input) : SV_Target
{
    float2 screenPos = input.pPos.xy / input.pPos.w;
    float2 captureTex = float2(
        1 - (screenPos.x + 1) * 0.5f,
        1 - (screenPos.y + 1) * 0.5f
    );
    float4 color = uiTexture.Sample(sampState, float3(captureTex, sliceIdx));
    
    return color;
}