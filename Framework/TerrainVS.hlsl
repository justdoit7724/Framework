
Texture2D heightMap;

#include "ShaderSampLinearPoint.cginc"

struct VS_Input
{
    float3 wPos : POSITION;
    float2 tex : TEXCOORD0;
    float2 boundsY : TEXCOORD1;
};
struct VS_OUTPUT
{
    float3 wPos : POSITION;
    float2 tex : TEXCOORD0;
    float2 boundsY : TEXCOORD1;
};

VS_OUTPUT main(VS_Input input)
{
    VS_OUTPUT output;
    
    output.wPos = input.wPos;
    output.wPos.y = heightMap.SampleLevel(linearPointSamp, input.tex, 0).r;
    output.boundsY = input.boundsY;
    output.tex = input.tex;

    return output;
}