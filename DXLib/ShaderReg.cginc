
#ifndef _SHADER_REG
#define _SHADER_REG

#define SHADER_REG_TRANSFORM_WORLD\
cbuffer TRANSFORM_WORLD : register(b0)\
{\
float4x4 transformWorld;\
}
#define SHADER_REG_CB_DIRLIGHT register(b0)
#define SHADER_REG_CB_POINTLIGHT register(b1)
#define SHADER_REG_CB_SPOTLIGHT register(b2)
#define SHADER_REG_CB_EYE\
cbuffer EYE : register(b3)\
{float4 eyePos;};
#define SHADER_REG_CB_MATERIAL register(b4)
#define SHADER_REG_CB_LIGHTVP register(b5)
#define SHADER_REG_CB_COLOR register(b6)

#define SHADER_REG_SRV_CM TextureCube cubeMap : register(t0);
#define SHADER_REG_SRV_DCM register(t1)

/*
material info
srvSubMat... = list of sub-materials which are diff and normal srv
and max number is 5
cbSubMatIndex = current index to access
*/
#define SHADER_REG_SRV_MATERIAL \
Texture2D srvSubMatDiff : register(t10);\
Texture2D srvSubMatNorm : register(t11);\
Texture2D srvSubMatExtra0 : register(t12);\
Texture2D srvSubMatExtra1 : register(t13);\
Texture2D srvSubMatExtra2 : register(t14);\
Texture2D srvSubMatExtra3 : register(t15);\
Texture2D srvSubMatExtra4 : register(t16);\
Texture2D srvSubMatExtra5 : register(t17);\
Texture2D srvSubMatExtra6 : register(t18);\
Texture2D srvSubMatExtra7 : register(t19);


#define SHADER_REG_SRV_SSAO register(t20)
#define SHADER_REG_SRV_SHADOW register(t21)
#define SHADER_REG_SRV_SHADOW_TRANSP register(t22)
#define SHADER_REG_SRV_DISPLACE register(t23)
#define SHADER_REG_SRV_ROUGHNESS register(t24)
#define SHADER_REG_SRV_METALLIC register(t25)
#define SHADER_REG_SRV_DEPTH Texture2D<float2> texPrevDepth : register(t26);

#define SHADER_REG_SAMP_POINT SamplerState pointSamp : register(s0);
#define SHADER_REG_SAMP_LINEAR SamplerState linearSamp : register(s1);
#define SHADER_REG_SAMP_LINEAR_POINT  SamplerState linPtSamp : register(s3);
#define SHADER_REG_SAMP_ANISOTROPIC SamplerState anisoSamp : register(s4);
#define SHADER_REG_SAMP_CMP_POINT SamplerComparisonState pointCmpSamp : register(s5)
#define SHADER_REG_SAMP_CMP_LINEAR_POINT SamplerComparisonState linPtCmpSamp : register(s6)

#endif