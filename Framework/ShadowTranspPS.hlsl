
#include "ShaderReg.cginc"

struct PS_INPUT
{
    float4 pos : SV_Position;
    float3 wNormal : TEXCOORD0;
    float pDist : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_Target
{
    return float4(normalize(input.wNormal), input.pDist);
}