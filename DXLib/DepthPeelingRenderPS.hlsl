
#include "ShaderInfo.cginc"
#include "ShaderReg.cginc"

Texture2D diffuseTex : SHADER_REG_SRV_DIFFUSE;
Texture2D depthTex : SHADER_REG_SRV_DEPTH;

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD1;
    float4 pPos : TEXCOORD2;
    float3 normal : TEXCOORD3;
    float2 tex : TEXCOORD4;
};

struct PS_OUTPUT
{
    float4 color : SV_Target;
    float depth : SV_Depth;
};
PS_OUTPUT main(PS_INPUT input)
{
    const float2 winSize = float2(600, 600);
    float2 tex = input.pPos.xy / input.pPos.w;
    tex = (tex + 1) * 0.5;
    tex.y = 1 - tex.y;
    int2 mTex = int2(
        min(winSize.x - 1, tex.x * winSize.x),
        min(winSize.y - 1, tex.y * winSize.y));
    
    float curDepth = depthTex.Load(int3(mTex, 0)).x;
    float4 mainTex = diffuseTex.Load(int3(mTex, 0));
    
    PS_OUTPUT output;
    output.color = mainTex;
    output.depth = curDepth;
    return output;

}