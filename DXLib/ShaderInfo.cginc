
#ifndef _SHADER_INFO
#define _SHADER_INFO

#define EPSILON 0.0000001
#define PI 3.141592
#define PIDIV2 1.570796
#define PIDIV4 0.785398

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
