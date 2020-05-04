
#ifndef _SHADER_INFO
#define _SHADER_INFO

#define EPSILON 0.000001f

float2 Lerp(float2 a, float2 b, float t)
{
    return a + t * (b - a);

}

float3 Lerp(float3 a, float3 b, float t)
{
    return a + t * (b - a);
}
float4 Lerp(float4 a, float4 b, float t)
{
    return a + t * (b - a);
}

#endif
