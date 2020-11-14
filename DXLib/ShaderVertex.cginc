
#ifndef _SHADER_VERTEX
#define _SHADER_VERTEX

struct STD_VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    //float3 tangent : TANGENT;
};

#endif