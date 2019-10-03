

#define EPSILON 0.000001f

float4 Lerp(float4 a, float4 b, float t)
{
    return a + t * (b - a);
}

struct STD_VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    float3 tangent : TANGENT;
};