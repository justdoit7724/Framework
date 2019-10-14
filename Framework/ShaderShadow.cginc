
float CalcShadowFactor(SamplerComparisonState samp, Texture2D map, float4 pLightPos, float mapWidth, float mapHeight)
{
    float2 shadowUI = pLightPos.xy / pLightPos.w;
    float lightDepth = pLightPos.z / pLightPos.w;

    float dx = 1.0f / mapWidth;
    float dy = 1.0f / mapHeight;

    float percentLit = 0;
    float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0, -dy), float2(dx, -dy),
        float2(-dx, 0), float2(0, 0), float2(dx, 0),
        float2(-dx, dy), float2(0, dy), float2(dx, dy)
    };
    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        percentLit += map.SampleCmpLevelZero(samp, shadowUI+offsets[i], lightDepth);
    }

    return (percentLit) * 0.111f;
}