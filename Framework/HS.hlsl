struct VS_OUT
{
    float3 pos : TEXCOORD0;
};

struct Patch
{
    float edgeTessFactor[4] : SV_TessFactor;
    float inTessFactor[2] : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
    float3 pos : TEXCOORD0;
};



Patch ConstantHS(InputPatch<VS_OUT, 3> patch, uint pI : SV_PrimitiveID)
{
    Patch p;
    p.edgeTessFactor[0] = 3.0f;
    p.edgeTessFactor[1] = 3.0f;
    p.edgeTessFactor[2] = 3.0f;
    p.inTessFactor[0] = 3.0f;
    p.inTessFactor[1] = 3.0f;

    return p;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(16.0f)]
HS_OUTPUT ControlPointHS(InputPatch<VS_OUT, 4> patch, uint i : SV_OutputControlPointID, uint pI : SV_PrimitiveID)
{
    HS_OUTPUT output;
    output.pos = patch[i].pos;

    return output;
}