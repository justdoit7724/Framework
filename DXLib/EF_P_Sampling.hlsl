
#include "ShaderInfo.cginc"
#include "ShaderReg.cginc"
#include "ShaderSampPoint.cginc"
#include "Geometrics.cginc"

cbuffer CB_Sample : register(b7)
{
    float cbSample;
    float cbVisual;
    float2 pad;
}


cbuffer DIRECTIONAL_LIGHT : register(b8)
{
    float4 cbShape1[6];
    float4 cbShape2[6];
};

cbuffer CB_PRIMITIVE : register(b9)
{
    float4 cbVerticeShape1[4];
    float4 cbVerticeShape2[4];
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};
float4 main(PS_INPUT input) : SV_Target
{
    const int iWidth = 12;
    const int iHeight = 12;
    const float radDot = 0.05f;
    const float3 col1 = float3(1, 0, 0);
    const float3 col2 = float3(0, 1, 0);
    
    float2 tex = input.tex;
    float2 texFrac = float2(frac(tex.x * iWidth), frac(tex.y * iHeight));
    int2 texIndex = int2(tex.x * iWidth, tex.y * iHeight);
    
    float3 final = 0;
    if (texFrac.x <= 0.015f || texFrac.y <= 0.015f || texFrac.x >= 0.985f || texFrac.y >= 0.985f)
        final += 0.75f;
    
    int nSample = cbSample;
    float rgfSampleX[18];
    float rgfSampleY[18];
    switch (nSample)
    {
        case 1:
            {
                rgfSampleX[0] = 0.5f;
                rgfSampleY[0] = 0.5f;
            }
            break;
        case 2:
            {
                rgfSampleX[0] = 0.25f;
                rgfSampleY[0] = 0.25f;
                rgfSampleX[1] = 0.75f;
                rgfSampleY[1] = 0.75f;
            }
            break;
        case 4:
            {
                rgfSampleX[0] = 0.375f;
                rgfSampleY[0] = 0.125f;
                rgfSampleX[1] = 0.875f;
                rgfSampleY[1] = 0.375f;
                rgfSampleX[2] = 0.125f;
                rgfSampleY[2] = 0.625f;
                rgfSampleX[3] = 0.635f;
                rgfSampleY[3] = 0.875f;
            }
            break;
        case 8:
            {
                rgfSampleX[0] = 0.5625f;
                rgfSampleY[0] = 0.3125f;
                rgfSampleX[1] = 0.4375f;
                rgfSampleY[1] = 0.6875f;
                rgfSampleX[2] = 0.8125f;
                rgfSampleY[2] = 0.5625f;
                rgfSampleX[3] = 0.3125f;
                rgfSampleY[3] = 0.1875f;
                rgfSampleX[4] = 0.1875f;
                rgfSampleY[4] = 0.8125f;
                rgfSampleX[5] = 0.0625f;
                rgfSampleY[5] = 0.4375f;
                rgfSampleX[6] = 0.6875f;
                rgfSampleY[6] = 0.9375f;
                rgfSampleX[7] = 0.9375f;
                rgfSampleY[7] = 0.0625f;
            }
            break;
        case 16:
            {
                rgfSampleX[0] = 0.5625f;
                rgfSampleY[0] = 0.5625f;
                rgfSampleX[1] = 0.4375f;
                rgfSampleY[1] = 0.3125f;
                rgfSampleX[2] = 0.3125f;
                rgfSampleY[2] = 0.6250f;
                rgfSampleX[3] = 0.7500f;
                rgfSampleY[3] = 0.4375f;
            
                rgfSampleX[4] = 0.1875f;
                rgfSampleY[4] = 0.3750f;
                rgfSampleX[5] = 0.6250f;
                rgfSampleY[5] = 0.8125f;
                rgfSampleX[6] = 0.8125f;
                rgfSampleY[6] = 0.6875f;
                rgfSampleX[7] = 0.6875f;
                rgfSampleY[7] = 0.1875f;
            
                rgfSampleX[8] = 0.3750f;
                rgfSampleY[8] = 0.8750f;
                rgfSampleX[9] = 0.5000f;
                rgfSampleY[9] = 0.0625f;
                rgfSampleX[10] = 0.2500f;
                rgfSampleY[10] = 0.1250f;
                rgfSampleX[11] = 0.1250f;
                rgfSampleY[11] = 0.7500f;
            
                rgfSampleX[12] = 0.0000f;
                rgfSampleY[12] = 0.5000f;
                rgfSampleX[13] = 0.9375f;
                rgfSampleY[13] = 0.2500f;
                rgfSampleX[14] = 0.8750f;
                rgfSampleY[14] = 0.9375f;
                rgfSampleX[15] = 0.0625f;
                rgfSampleY[15] = 0.0000f;
            
                nSample++;
                rgfSampleX[16] = 1.0000f;
                rgfSampleY[16] = 0.5000f;
                nSample++;
                rgfSampleX[17] = 0.0625f;
                rgfSampleY[17] = 1.0000f;

            }
            break;
    }
    
    float2 fCSample = 0;
    int iCIndex = 0;
    float fSqrCDist = 9999;
    for (int i = 0; i < cbSample; ++i)
    {
        float curDist = pow(texFrac.x - rgfSampleX[i], 2) + pow(texFrac.y - rgfSampleY[i], 2);
        if (curDist < fSqrCDist)
        {
            fCSample = float2(rgfSampleX[i], rgfSampleY[i]);
            fSqrCDist = curDist;
            iCIndex = i;

        }
    }
    
    int rgnTotal1[16];
    int rgnTotal2[16];
    for (i = 0; i < cbSample; ++i)
    {
        rgnTotal1[i] = 0;
        rgnTotal2[i] = 0;
    }
    for (i = 0; i < cbSample; ++i)
    {
        float2 curSampleClipPt = float2((texIndex.x + rgfSampleX[i]) / iWidth, (texIndex.y + rgfSampleY[i]) / iHeight);
        curSampleClipPt -= 0.5f;
        curSampleClipPt *= 2;
        curSampleClipPt.y *= -1;
    
        [unroll(4)]
        for (int j = 0; j < 4; ++j)
        {
            int indexA = j;
            int indexB = j + 1;
            if (indexB == 4)
                indexB = 0;
    
            rgnTotal1[i] += Intersect2DRayLine(curSampleClipPt, float2(0, 1), cbVerticeShape1[indexA].xy, cbVerticeShape1[indexB].xy);
            rgnTotal2[i] += Intersect2DRayLine(curSampleClipPt, float2(0, 1), cbVerticeShape2[indexA].xy, cbVerticeShape2[indexB].xy);
        }
    }
    int nTotal1=0;
    int nTotal2=0;
    for (i = 0; i < cbSample; ++i)
    {
        if (rgnTotal2[i] & 1)
            nTotal2++;
        else if (rgnTotal1[i] & 1)
            nTotal1++;
    }
    
    float3 col = 0;
    if (rgnTotal1[iCIndex] & 1)
        col = col1;
    if (rgnTotal2[iCIndex] & 1)
        col = col2;
    float3 aaCol = (nTotal1 * col1 + nTotal2 * col2) / cbSample;
    final += aaCol;
    
    float distT = sqrt(fSqrCDist) / radDot;
    float t = ceil(1 - saturate(distT));
    
    if ((rgnTotal1[iCIndex] & 1) || (rgnTotal2[iCIndex] & 1))
    {
        final = lerp(final, col, t);
    
        final = lerp(final, float3(0.5, 0.5, 0.5), (0.5 < distT) && (distT < 1));
    }
    else
    {
        final = lerp(final, float3(0.5, 0.5, 0.5), (0.0 < distT) && (distT < 0.5));
    }
    
    return float4(final, 1);
}