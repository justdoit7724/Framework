
#include "ShaderInfo.cginc"
#include "ShaderSampLinearPoint.cginc"

struct DS_Input
{
    float3 wPos : POSITION;
    float2 tex : TEXCOORD;
};
struct DS_Output
{
    float4 pos : SV_POSITION;
    float3 wPos : POSITION;
    float2 tex : TEXCOORD0;
    float2 tiledTex : TEXCOORD1;
};
struct PatchTess
{
    float edgeTess[4] : SV_TessFactor;
    float insideTess[2] : SV_InsideTessFactor;
};

float2 texScale = 50.0f;
Texture2D heightMap;

cbuffer CB_TRANSFORM : register(b0)
{
    column_major float4x4 gViewProj;
}

[domain("quad")]
DS_Output main(PatchTess patchTess, float2 uv : SV_DomainLocation, const OutputPatch<DS_Input,4> quad)
{
    DS_Output output;
    
    output.wPos = Lerp(
    lerp(quad[0].wPos, quad[1].wPos, uv.x),
    lerp(quad[2].wPos, quad[3].wPos, uv.x),
    uv.y);
    
    output.tex = Lerp(
    Lerp(quad[0].tex, quad[1].tex, uv.x),
    Lerp(quad[2].tex, quad[3].tex, uv.x),
    uv.y);
    
    output.wPos.y = heightMap.SampleLevel(linearPointSamp, output.tex, 0);

    output.tiledTex = output.tex * texScale;
    
    output.pos = mul(gViewProj, float4(output.wPos, 1));
    
    return output;
}