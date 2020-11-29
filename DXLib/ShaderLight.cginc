
#include "ShaderInfo.cginc"
#include "ShaderReg.cginc"

#define LIGHT_MAX_EACH 3 // multiple lights can be applied
#define LIGHT_SPEC_POWER_MAX 16

cbuffer DIRECTIONAL_LIGHT : SHADER_REG_CB_DIRLIGHT
{
    float4 d_Ambient[LIGHT_MAX_EACH];
    float4 d_Diffuse[LIGHT_MAX_EACH];
    float4 d_Specular[LIGHT_MAX_EACH];
    float4 d_Dir[LIGHT_MAX_EACH];
    float4 d_Enabled[LIGHT_MAX_EACH];
    float4 d_intensity[LIGHT_MAX_EACH];
};
cbuffer POINT_LIGHT : SHADER_REG_CB_POINTLIGHT
{
    float4 p_Ambient[LIGHT_MAX_EACH];
    float4 p_Diffuse[LIGHT_MAX_EACH];
    float4 p_Specular[LIGHT_MAX_EACH];
    float4 p_Pos[LIGHT_MAX_EACH];
    float4 p_Info[LIGHT_MAX_EACH]; // enabled,range
    float4 p_Att[LIGHT_MAX_EACH];
    float4 p_Intensity[LIGHT_MAX_EACH];
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
    float4 s_Intensity[LIGHT_MAX_EACH];
};
cbuffer MATERIAL : SHADER_REG_CB_MATERIAL
{
    float4 mDiffuse;
    float4 mAmbient;
    float4 mSpecular;
};
void ComputeDirectionalLight(float3 n, float3 v, out float3 ambient, out float3 diffuse, out float3 spec)
{
    diffuse = float3(0, 0, 0);
    spec = float3(0, 0, 0);
    ambient = float3(0, 0, 0);
    
    for (int i = 0; i < LIGHT_MAX_EACH; ++i)
    {
        ambient += d_Enabled[i].x * d_intensity[i].x * mAmbient.xyz * d_Ambient[i].xyz;
        diffuse += d_Enabled[i].x * d_intensity[i].x * saturate(dot(-d_Dir[i].xyz, n)) * mDiffuse.xyz * d_Diffuse[i].xyz;
    
        float3 r = reflect(d_Dir[i].xyz, n);
        float specFactor = pow(max(EPSILON,dot(r, v)), LIGHT_SPEC_POWER_MAX);
        spec += d_Enabled[i].x * d_intensity[i].x * specFactor * mSpecular.xyz * d_Specular[i].xyz;
    }
}
void ComputePointLight(float3 pos, float3 n, float3 v, out float3 ambient, out float3 diffuse, out float3 spec)
{
    diffuse = float3(0, 0, 0);
    spec = float3(0, 0, 0);
    ambient = float3(0, 0, 0);
    
    for (int i = 0; i < LIGHT_MAX_EACH; ++i)
    {
        float3 l = p_Pos[i].xyz - pos;
        float d = length(l);
        
        l /= d; // normalize
        float diffuseFactor = max(0.0f, dot(l, n));
        
        float3 r = reflect(-v, n);
        float specFactor = pow(saturate(dot(r, l)), LIGHT_SPEC_POWER_MAX);
        float att = p_Intensity[i].x / (d*d + 1);
        
        ambient += p_Info[i].x * mAmbient.xyz * p_Ambient[i].xyz * att;
        diffuse += p_Info[i].x * diffuseFactor * mDiffuse.xyz * p_Diffuse[i].xyz * att;
        spec += p_Info[i].x * specFactor * mSpecular.xyz * p_Specular[i].xyz * att;
    }
}
void ComputeSpotLight(float3 pos, float3 normal, float3 view, out float3 ambient, out float3 diffuse, out float3 spec)
{
    diffuse = float3(0, 0, 0);
    spec = float3(0, 0, 0);
    ambient = float3(0, 0, 0);
    
    for (int i = 0; i < LIGHT_MAX_EACH; ++i)
    {
        float3 tmpA = 0;
        float3 tmpD = 0;
        float3 tmpS = 0;

        float3 l = s_Pos[i].xyz - pos;
        float d = length(l);

        l /= d;
        float diffuseFactor = saturate(dot(l, normal));
        
        float3 r = reflect(-l, normal);
        float specFactor = pow(max(EPSILON, saturate(dot(r, view))), LIGHT_SPEC_POWER_MAX);
     
        float spot = 1.0 - saturate(acos(saturate(dot(-l, s_Dir[i].xyz))) / (s_info[i].z + EPSILON));
        spot = -pow(min(-EPSILON, spot - 1), 2) + 1;
        float att = spot * s_Intensity[i].x / (d * d + EPSILON);
        tmpD = diffuseFactor * mDiffuse.xyz * s_Diffuse[i].xyz * att;
        tmpS = specFactor * mSpecular.xyz * s_Specular[i].xyz * att;
        tmpA = mAmbient.xyz * s_Ambient[i].xyz * spot;
        
        ambient += s_info[i].x * tmpA;
        diffuse += s_info[i].x*tmpD;
        
        spec += s_info[i].x * tmpS;

    }
}
