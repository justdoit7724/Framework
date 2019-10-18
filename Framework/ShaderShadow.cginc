
float CalcShadowFactor(float3 wNormal, float3 lightDir, SamplerComparisonState samp, Texture2D map, float4 pLightPos, float mapWidth, float mapHeight)
{
    if (dot(wNormal, lightDir)>0)
    {
        return 1;
    }

    float3 lightPos = pLightPos.xyz / pLightPos.w;

    float percentLit = 0;
    float mapDX = 1.0f / mapWidth;
    float mapDY = 1.0f / mapHeight;
    float2 offsets[9] =
    {
        float2(-mapDX, -mapDY), float2(0, -mapDY), float2(mapDX, -mapDY),
        float2(-mapDX, 0), float2(0, 0), float2(mapDX, 0),
        float2(-mapDX, mapDY), float2(0, mapDY), float2(mapDX, mapDY)
    };
    //float ddx_z = ddx_fine(lightPos).z * mapDX * mapWidth;
    //float ddy_z = ddy_fine(lightPos).z * mapDY * mapHeight;

    //float zOffset[9] =
    //{
    //    -ddx_z - ddy_z, -ddy_z, ddx_z - ddy_z,
    //    -ddx_z, 0, ddx_z,
    //    -ddx_z + ddy_z, ddy_z, ddx_z + ddy_z
    //};
    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        percentLit += map.SampleCmpLevelZero(samp, lightPos.xy + offsets[i], lightPos.z).r;
    }
    
    return (percentLit) * 0.111f;
}