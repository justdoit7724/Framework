
#include "ShaderReg.cginc"

float minDist;
float maxDist;

float minTess;
float maxTess;

TEST

float CalcTessFactor(float3 pt)
{
    float dist = distance(pt, eyePos.xyz);
    
    float s = saturate((dist - minDist) / (maxDist - minDist));

    return pow(2, lerp(maxTess, minTess, s));
}

struct PatchTess
{
    float edgeTess[4] : SV_TessFactor;
    float insideTess[2]  : SV_InsideTessFactor;
};

struct HS_Input
{
    float3 wPos : POSITION;
    float2 tex : TEXCOORD0;
    float2 boundsY : TEXCOORD1;
};
struct HS_Output
{
    float3 wPos : POSITION;
    float2 tex : TEXCOORD;

};

PatchTess ConstantHS(InputPatch<HS_Input, 4> patch, uint patchID:SV_PrimitiveID)
{
    PatchTess pt;
    
    float3 e0 = (patch[0].wPos + patch[2].wPos) * 0.5f;
    float3 e1 = (patch[0].wPos + patch[1].wPos) * 0.5f;
    float3 e2 = (patch[1].wPos + patch[3].wPos) * 0.5f;
    float3 e3 = (patch[2].wPos + patch[3].wPos) * 0.5f;
    float3 c = (patch[0].wPos + patch[1].wPos + patch[2].wPos + patch[3].wPos)*0.25f;
    
    pt.edgeTess[0] = CalcTessFactor(e0);
    pt.edgeTess[1] = CalcTessFactor(e1);
    pt.edgeTess[2] = CalcTessFactor(e2);
    pt.edgeTess[3] = CalcTessFactor(e3);

    pt.insideTess[0] = CalcTessFactor(c);
    pt.insideTess[1] = pt.insideTess[0];
    
    return pt;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("point")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
HS_Output main(InputPatch<HS_Input, 4> patch, uint i : SV_OutputControlPointID)
{
    HS_Output output;
    
    output.wPos = patch[i].wPos;
    output.tex = patch[i].tex;
    return output;
}


