
#include "ShaderInfo.cginc"
#include "ShaderReg.cginc"

Texture2D<float2> texPrevDepth : register(t10);

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
    //float depth : SV_Depth;
};

PS_OUTPUT main(PS_INPUT input)
{
    const float2 winSize = float2(600, 600);
    float2 tex = input.pPos.xy / input.pPos.z;
    tex = (tex + 1)*0.5;
    tex.y = 1 - tex.y;
    int2 mTex = int2(
        min(winSize.x - 1, tex.x * winSize.x),
        min(winSize.y - 1, tex.y * winSize.y));
    
    float newDepth = input.pPos.z / input.pPos.w;
    float curDepth = texPrevDepth.Load(int3(mTex, 0), 0).x;
    
    //clip(newDepth - curDepth - EPSILON);
    
    PS_OUTPUT output;
    output.color = float4(tex, 0, 0);
    //output.depth = newDepth;
    return output;

}