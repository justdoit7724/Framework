
#ifndef _SHADER_VERTEX
#define _SHADER_VERTEX

struct STD_VS_INPUT
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
    float3 normal : NORMAL;
    //float3 tangent : TANGENT;
};

#endif