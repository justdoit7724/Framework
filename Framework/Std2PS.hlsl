
#include "ShaderInfo.cginc"
#include "ShaderLight.cginc"
#include "ShaderReg.cginc"
#include "ShaderSampPoint.cginc"
#include "ShaderSampCmpPoint.cginc"
#include "ShaderRghMetal.cginc"

#define REFRACTION_INDEX_GLASS 1.2

cbuffer EYE : SHADER_REG_CB_EYE
{
    float4 eyePos;
};
cbuffer LIGHT_PERSPECTIVE : SHADER_REG_CB_LIGHTVP
{
    float4x4 lightVPT;
};

TextureCube cmTex : SHADER_REG_SRV_CM;
Texture2D diffuseTex : SHADER_REG_SRV_DIFFUSE;
Texture2D shadowTex : SHADER_REG_SRV_SHADOW;
Texture2D shadowTranspTex : SHADER_REG_SRV_SHADOW_TRANSP;
//...

float3 ComputeTransparency(float3 wPos, float3 normal, float3 look)
{
    float3 tex3d = Lerp(look, normalize(look - normal), saturate(REFRACTION_INDEX_GLASS - 1));
    return cmTex.SampleLevel(pointSamp, tex3d, 0).rgb;
}
float3 ComputeReflect(float3 wNormal, float3 look)
{
    return cmTex.SampleLevel(pointSamp, reflect(look, wNormal), 0).xyz;
}

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
    
    return saturate(1-shadowTex.SampleCmpLevelZero(pointCmpSamp, lightPos.xy, lightPos.z).r);
}
float2 DirectionalLightTranspShadowFactor(float3 wDir, float3 lightDir, float3 wPos)
{
    float4 pLightPos = mul(lightVPT, float4(wPos, 1));
    float3 lightPerspective = pLightPos.xyz / pLightPos.w;

    float4 centerSample = shadowTranspTex.SampleLevel(pointSamp, lightPerspective.xy, 0);
    float3 centerWDir = centerSample.xyz;
    float centerPDist = centerSample.w;

    float power = saturate(pow(3 * dot(-wDir, lightDir), 3));
    float overlap = saturate((lightPerspective.z - centerPDist) * 100);
    
    float totalIntensity = 1 - dot(-centerWDir, lightDir);
    
    
    float area = saturate(ceil(lightPerspective.z - centerPDist));
    return float2(power * overlap * totalIntensity, area);
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 pPos : TEXCOORD0;
    float3 wPos : TEXCOORD1;
    float3 normal : TEXCOORD2;
    float2 tex : TEXCOORD3;
    float3 tangent : TEXCOORD4;
};
float4 main(PS_INPUT input) : SV_Target
{
    float3 wNormal = normalize(input.normal);

    float3 look = normalize(input.wPos - eyePos.xyz);
    
    float3 ambient = 0;
    float3 diffuse = 0;
    
    float3 specular = 0;
    
    float roughness = ComputeRoughness(pointSamp, input.tex);
    float metallic = ComputeMetallic(pointSamp, input.tex);
    ComputeDirectionalLight(wNormal, -look, roughness, ambient, diffuse, specular);
    specular *= (1 - roughness);
    float transpT = mDiffuse.a * (1 - roughness);
    float refractT = metallic * (1 - roughness);

    float3 transp = ComputeTransparency(input.wPos, wNormal, look);
    float3 tex = diffuseTex.Sample(pointSamp, input.tex).xyz;
    float3 reflec = ComputeReflect(wNormal, look);
    
    float oShadowFactor = DirectionalLightOpaqueShadowFactor(wNormal, d_Dir[0].xyz, input.wPos);
    float2 tShadowFactor = DirectionalLightTranspShadowFactor(wNormal, d_Dir[0].xyz, input.wPos);
    float shadowFactor = saturate(1 - max(oShadowFactor, tShadowFactor.x));
    specular = specular * shadowFactor + (saturate(0.05f - tShadowFactor.x) * tShadowFactor.y);
    ambient *= tex;
    diffuse *= tex * shadowFactor;
    
    float3 surface = Lerp(ambient + diffuse, transp, transpT);
    surface = Lerp(surface, reflec, refractT);
    
    return float4(specular + surface, 1);
}