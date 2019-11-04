
#define EPSILON 0.000001f

float4 Lerp(float4 a, float4 b, float t)
{
    return a + t * (b - a);
}
