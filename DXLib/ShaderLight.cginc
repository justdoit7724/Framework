
#ifndef _SHADER_LIGHT
#define _SHADER_LIGHT

#include "ShaderReg.cginc"

#define LIGHT_ENABLED 1
#define LIGHT_DISABLED 0
#define LIGHT_MAX_EACH 1 // multiple lights can be applied
#define LIGHT_SPEC_POWER_MAX 16

cbuffer DIRECTIONAL_LIGHT : SHADER_REG_CB_DIRLIGHT
{
    float4 d_Ambient[LIGHT_MAX_EACH];
    float4 d_Diffuse[LIGHT_MAX_EACH];
    float4 d_Specular[LIGHT_MAX_EACH];
    float4 d_Dir[LIGHT_MAX_EACH];
    float4 d_Enabled[LIGHT_MAX_EACH];
};
cbuffer POINT_LIGHT : SHADER_REG_CB_POINTLIGHT
{
    float4 p_Ambient[LIGHT_MAX_EACH];
    float4 p_Diffuse[LIGHT_MAX_EACH];
    float4 p_Specular[LIGHT_MAX_EACH];
    float4 p_Pos[LIGHT_MAX_EACH];
    // enabled,range
    float4 p_Info[LIGHT_MAX_EACH]; 
    float4 p_Att[LIGHT_MAX_EACH];
};
cbuffer SPOT_LIGHT : SHADER_REG_CB_SPOTLIGHT
{
    float4 s_Ambient[LIGHT_MAX_EACH];
    float4 s_Diffuse[LIGHT_MAX_EACH];
    float4 s_Specular[LIGHT_MAX_EACH];
    float4 s_Pos[LIGHT_MAX_EACH];
   
    float4 s_info[LIGHT_MAX_EACH]; // enabled, range, rad, spot
    float4 s_Dir[LIGHT_MAX_EACH];
    float4 s_Att[LIGHT_MAX_EACH];
};
cbuffer MATERIAL : register(b4)
{
    float4 mDiffuse;
    float4 mAmbient;
    float4 mSpecular;
};
void ComputeDirectionalLight(float3 normal, float3 toEye, out float3 ambient, out float3 diffuse, out float3 spec)
{
    ambient = float3(0.0f, 0.0f, 0.0f);
    diffuse = float3(0.0f, 0.0f, 0.0f);
    spec = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < LIGHT_MAX_EACH; ++i)
    {
        if (d_Enabled[i].x == LIGHT_DISABLED)
            continue;
        
        ambient += mAmbient.xyz * d_Ambient[i].xyz;
        diffuse += saturate(dot(-d_Dir[i].xyz, normal)) * mDiffuse.xyz * d_Diffuse[i].xyz;
    
        float3 v = reflect(d_Dir[i].xyz, normal);
        float specFactor = pow(saturate(dot(v, toEye)), LIGHT_SPEC_POWER_MAX);
        spec += specFactor * mSpecular.xyz * d_Specular[i].xyz;
    }
}

// not used in the project
void ComputePointLight(float3 pos, float3 normal, float3 toEye, out float3 ambient, out float3 diffuse, out float3 spec)
{
    ambient = float3(0.0f, 0.0f, 0.0f);
    diffuse = float3(0.0f, 0.0f, 0.0f);
    spec = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < LIGHT_MAX_EACH; ++i)
    {
        if (p_Info[i].x == LIGHT_DISABLED)
            continue;
        
        float3 tmpD = 0;
        float3 tmpS = 0;

        float3 lightVec = p_Pos[i].xyz - pos;
        float d = length(lightVec);
        
        lightVec /= d; // normalize
        float diffuseFactor = max(0.0f, dot(lightVec, normal));
        
        float3 hVec = normalize(toEye + lightVec);
        float specFactor = pow(saturate(dot(hVec, normal)), mSpecular.w);
        float att = 1.0f / dot(p_Att[i].xyz, float3(1.0f, d, d * d));
        
        tmpD = diffuseFactor * mDiffuse.xyz * p_Diffuse[i].xyz * att;
        tmpS = specFactor * mSpecular.xyz * p_Specular[i].xyz * att;

        ambient += mAmbient.xyz * p_Ambient[i].xyz;
        diffuse += tmpD;
        spec += tmpS;
    }
}
void ComputeSpotLight(float3 pos, float3 normal, float3 toEye, out float3 ambient, out float3 diffuse, out float3 spec)
{
    ambient = float3(0.0f, 0.0f, 0.0f);
    diffuse = float3(0.0f, 0.0f, 0.0f);
    spec = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < LIGHT_MAX_EACH; ++i)
    {
        if (s_info[i].x == LIGHT_DISABLED)
            continue;
        
        float3 tmpA = 0;
        float3 tmpD = 0;
        float3 tmpS = 0;

        float3 lightVec = s_Pos[i].xyz - pos;
        float d = length(lightVec);

        if (d > s_info[i].y)
            continue;
    
        lightVec /= d;
        float diffuseFactor = saturate(dot(lightVec, normal));
        
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(saturate(dot(v, toEye)), mSpecular.w);
        float spot = pow(saturate(dot(-lightVec, s_Dir[i].xyz)), s_info[i].w);
        float att = spot / dot(s_Att[i].xyz, float3(1.0f, d, d * d));
        tmpD = diffuseFactor * mDiffuse.xyz * s_Diffuse[i].xyz * att;
        tmpS = specFactor * mSpecular.xyz * s_Specular[i].xyz * att;
        tmpA = mAmbient.xyz * s_Ambient[i].xyz * spot;
        
        ambient += tmpA;
        diffuse += tmpD;
        spec += tmpS;
    }
}

#endif