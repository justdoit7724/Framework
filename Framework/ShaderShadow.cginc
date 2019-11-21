
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

    float percentLit = 0;
    //float mapDX = 1.0f / mapWidth;
    //float mapDY = 1.0f / mapHeight;
    //float2 offsets[9] =
    //{
    //    float2(-mapDX, -mapDY), float2(0, -mapDY), float2(mapDX, -mapDY),
    //    float2(-mapDX, 0), float2(0, 0), float2(mapDX, 0),
    //    float2(-mapDX, mapDY), float2(0, mapDY), float2(mapDX, mapDY)
    //};
    
    //[unroll]
    //for (int i = 0; i < 9; ++i)
    //{
    //}
    percentLit += shadowTex.SampleCmpLevelZero(shadowSamp, lightPos.xy /*+ offsets[i]*/, lightPos.z).r;
    
    return percentLit;
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
