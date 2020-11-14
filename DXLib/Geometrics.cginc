
#ifndef _SHADER_GEOMETRICS
#define _SHADER_GEOMETRICS

int Intersect2DRayLine(float2 rayOrigin, float2 rayDir, float2 linePt1, float2 linePt2)
{
    float2 o = rayOrigin;
    float2 d = rayDir;
    float2 a = linePt1;
    float2 b = linePt2;

    
    float denom1 = d.x * (b.y - a.y) - d.y * (b.x - a.x);
    float denom2 = (b.x - a.x) * d.y - (b.y - a.y) * d.x;
    
    if ((denom1 * denom2) == 0)
        return 0;
    
    float t1 = ((a.x - o.x) * (b.y - a.y) - (a.y - o.y) * (b.x - a.x)) / denom1;
    float t2 = ((o.x - a.x) * d.y - (o.y - a.y) * d.x) / denom2;
    
    return (t1 > 0 && 0 < t2 && t2 <= 1);
}

#endif
