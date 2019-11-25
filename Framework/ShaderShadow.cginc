
#include "ShaderReg.cginc"

cbuffer LIGHT_PERSPECTIVE : SHADER_REG_PS_CB_LIGHTVP
{
    float4x4 lightVPT;
};

Texture2D shadowTex : SHADER_REG_PS_SRV_SHADOW;
SamplerComparisonState shadowSamp : SHADER_REG_PS_SAMP_SHADOW;

float DirectionalLightShadowFactor(float3 wNormal, float3 lightDir, float3 wPos)
{
    if (dot(wNormal, lightDir)>0)
    {
        return 0;
    }

    float4 pLightPos = mul(lightVPT, float4(wPos, 1));
    float3 lightPos = pLightPos.xyz / pLightPos.w;

    float mapWidth, mapHeight;
    shadowTex.GetDimensions(mapWidth, mapHeight);
    float dx = 1.0f / mapWidth;
    float dy = 1.0f / mapHeight;
    float2 offsets[25] =
    {
        float2(-dx * 2, -dy * 2),   float2(-dx, -dy * 2), float2(0, -dy * 2),    float2(dx, -dy * 2), float2(dx * 2, -dy * 2),
        float2(-dx * 2, -dy), float2(-dx, -dy), float2(0, -dy), float2(dx, -dy), float2(dx * 2, -dy),
        float2(-dx*2, 0),          float2(-dx, 0),      float2(0, 0),         float2(dx, 0), float2(dx*2, 0),
        float2(-dx * 2, dy), float2(-dx, dy), float2(0, dy), float2(dx, dy), float2(dx * 2, dy),
        float2(-dx * 2, dy * 2),    float2(-dx, dy * 2),  float2(0, dy * 2),     float2(dx, dy * 2), float2(dx * 2, dy * 2)
    };
    
    float percentLit = 0;
    [unroll]
    for (int i = 0; i < 25; ++i)
    {
        percentLit += shadowTex.SampleCmpLevelZero(shadowSamp, lightPos.xy + offsets[i], lightPos.z).r;
    }
    
    return percentLit*0.04f;
}
float PointLightShadowFactor(float3 wNormal, float3 wPos, float3 lightPos, TextureCube map, SamplerState samp, float2 shadowPMatElem)
{
    float3 lPos = wPos - lightPos;
    
    if (dot(wNormal, lPos) > 0)
        return 1;
    
    float3 viewForward[6] =
    {
        float3(1, 0, 0),
        float3(-1, 0, 0),
        float3(0, 1, 0),
        float3(0, -1, 0),
        float3(0, 0, 1),
        float3(0, 0, -1)
    };
    float curDepth = 0;
    float3 curForward = 0;
    for (int i = 0; i < 6; ++i)
    {
        float newDepth = dot(viewForward[i], lPos);
        if (curDepth < newDepth)
        {
            curDepth = newDepth;
            curForward = viewForward[i];
        }
    }

    float vZ = curDepth;
    float pZ = shadowPMatElem.x + shadowPMatElem.y / vZ;
    
    return (map.Sample(samp, lPos).r>=pZ)?1:0;
}
