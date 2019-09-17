
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D<uint2> uiTexture : register(t0);

float4 main(PS_INPUT input) : SV_Target
{
    int2 idx = int2(380, 380);
    uint2 color = uiTexture[idx];
    float colorl = smoothstep(0, 0xff, color.y);

    return float4(colorl.xxx, 1);

}