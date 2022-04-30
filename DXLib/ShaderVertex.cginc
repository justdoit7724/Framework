
#ifndef _SHADER_VERTEX
#define _SHADER_VERTEX

#define BONE_MAX 128

struct STD_VS_INPUT
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
<<<<<<< HEAD
    uint4 boneId : BONEINDICE;
=======
    float3 normal : NORMAL;
>>>>>>> 03_DepthPeeling
    //float3 tangent : TANGENT;
};

#endif