
#define EPSILON 0.000001f

float3 Lerp(float3 a, float3 b, float t)
{
    return a + t * (b - a);
}
float4 Lerp(float4 a, float4 b, float t)
{
    return a + t * (b - a);
}
