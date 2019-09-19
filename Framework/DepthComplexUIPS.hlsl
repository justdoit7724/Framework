
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D<uint2> uiTexture : register(t0);

float4 main(PS_INPUT input) : SV_Target
{
    uint width = 0, height = 0;
    uiTexture.GetDimensions(width, height);

    int2 curIdx = int2(
		min(width * input.tex.x, width-1),
		min(height * (input.tex.y), height - 1)
	);
    if (uiTexture[curIdx].y > 10)
    {
        return float4(0.5f, 0, 0, 1);
    }
    float complex = uiTexture[curIdx].y/10.0f;
    float3 color = 0;
    if (complex < 0.5f)
    {
        color = float3(
            0,
            lerp(0, 1, complex),
            lerp(1, 0, complex)
        );
    }
    else
    {
        color = float3(
            lerp(0, 1, complex),
            lerp(1, 0, complex),
            0
        );
    }

    return float4(color, 1);

}