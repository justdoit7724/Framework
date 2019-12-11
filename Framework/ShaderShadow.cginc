
#include "ShaderReg.cginc"
#include "ShaderSampCmpLinearPoint.cginc"
#include "ShaderSampPoint.cginc"

cbuffer LIGHT_PERSPECTIVE : SHADER_REG_CB_LIGHTVP
{
    float4x4 lightVPT;
};


Texture2D shadowTex : SHADER_REG_SRV_SHADOW;
Texture2D shadowTranspTex : SHADER_REG_SRV_SHADOW_TRANSP;

float DirectionalLightOpaqueShadowFactor(float3 wNormal, float3 lightDir, float3 wPos)
{
    if (dot(wNormal, lightDir) > 0)
        return 0;
    
    float4 pLightPos = mul(lightVPT, float4(wPos, 1));
    float3 lightPos = pLightPos.xyz / pLightPos.w;

    float mapWidth, mapHeight;
    shadowTex.GetDimensions(mapWidth, mapHeight);
    float dx = 1.0f / mapWidth;
    float dy = 1.0f / mapHeight;

    float3 offsets[9] =
    {
        float3(-dx, -dy, 0.05), float3(0, -dy, 0.15), float3(dx, -dy, 0.05),
        float3(-dx, 0, 0.15), float3(0, 0, 0.2), float3(dx, 0, 0.15),
        float3(-dx, dy, 0.05), float3(0, dy, 0.15), float3(dx, dy, 0.05)
    };
    
    float totalFactor = 0;
    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        totalFactor += shadowTex.SampleCmpLevelZero(linearPointCmpSamp, lightPos.xy + offsets[i].xy, lightPos.z).r * offsets[i].z;
    }
    
    totalFactor = 1 - totalFactor;
    return saturate(totalFactor);
}
float2 DirectionalLightTranspShadowFactor(float3 wDir, float3 lightDir, float3 wPos)
{
    float4 pLightPos = mul(lightVPT, float4(wPos, 1));
    float3 lightPerspective = pLightPos.xyz / pLightPos.w;

    
    // center first
    float4 centerSample = shadowTranspTex.SampleLevel(pointSamp, lightPerspective.xy, 0);
    float3 centerWDir = centerSample.xyz;
    float centerPDist = centerSample.w;

    
    float power = saturate(pow(3 * dot(-wDir, lightDir), 3));
    
    float totalIntensity =  (1 - dot(-centerWDir, lightDir)) * 0.2f;
    float overlap = saturate((lightPerspective.z - centerPDist) * 100);
    float totalWeight = 0.2f;
    
    // 3x3 except center
    float mapWidth, mapHeight;
    shadowTranspTex.GetDimensions(mapWidth, mapHeight);
    float dx = 1.0f / mapWidth;
    float dy = 1.0f / mapHeight;
    float3 offsets[8] =
    {
        float3(-dx, -dy, 0.075f), float3(0, dy, 0.125f), float3(dx, dy, 0.075f),
        float3(-dx, 0, 0.125f), /*                 */float3(dx, 0, 0.125f),
        float3(-dx, dy, 0.075f), float3(0, dy, 0.125f), float3(dx, dy, 0.075f)
    };
    
    for (int i = 0; i < 8; ++i)
    {
        float4 shadowSample = shadowTranspTex.SampleLevel(pointSamp, lightPerspective.xy + offsets[i].xy, 0);
        float3 pWDir = shadowSample.xyz;
        float pDepth = shadowSample.w;
    
        if (abs(pDepth - centerPDist) <= 0.1)
        {
            totalIntensity += (1 - dot(-pWDir, lightDir)) * offsets[i].z;
            totalWeight += offsets[i].z;
        }
    }
    
    float area = saturate(ceil(lightPerspective.z - centerPDist) * dot(-wDir, lightDir));
    return float2(power * (overlap * totalWeight) * totalIntensity / totalWeight, area);
}

// 6 side cube shadow mapping (not used in this project though)
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
