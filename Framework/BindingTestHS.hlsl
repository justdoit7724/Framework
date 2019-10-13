struct HS_INPUT
{
    float3 pos : TEXCOORD0;
};
struct Patch
{
    float edgeTessFactor[3] : SV_TessFactor;
    float inTessFactor : SV_InsideTessFactor;
};
struct HS_OUTPUT
{
    float3 pos : TEXCOORD0;
};

cbuffer CB_HS : register(b0)
{
    float4 hs;
}

Patch ConstantHS(InputPatch<HS_INPUT, 3> patch, uint pi : SV_PrimitiveID)
{
    Patch p;
    p.edgeTessFactor[0] = 5;
    p.edgeTessFactor[1] = 5;
    p.edgeTessFactor[2] = 5;
    p.inTessFactor = 5;
    return p;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HS_OUTPUT main(InputPatch<HS_INPUT, 3> patch, uint i : SV_OutputControlPointID, uint pi : SV_PrimitiveID)
{
    HS_OUTPUT output;
    output.pos = patch[i].pos;

    return output;
}