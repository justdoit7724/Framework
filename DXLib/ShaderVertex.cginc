
#ifndef _SHADER_VERTEX
#define _SHADER_VERTEX

#define BONE_MAX 128

struct STD_VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    uint boneId : TEXCOORD1;
    //float3 tangent : TANGENT;
};

#endif