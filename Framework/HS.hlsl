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

cbuffer CB_EYE : register(b0)
{
    float3 eye_pos;
}

Patch ConstantHS(InputPatch<HS_INPUT, 3> patch, uint pI : SV_PrimitiveID)
{
    float3 center = (patch[0].pos + patch[1].pos + patch[2].pos)*0.333f;
    float dist = length(center - eye_pos);

    const float d0 = 40.0f;
    const float d1 = 200.0f;
    float tess = max(16.0f * saturate((d1 - dist) / (d1 - d0)),1);

    Patch p;
    p.edgeTessFactor[0] = tess;
    p.edgeTessFactor[1] = tess;
    p.edgeTessFactor[2] = tess;
    p.inTessFactor = tess;

    return p;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(32.0f)]
HS_OUTPUT main(InputPatch<HS_INPUT, 3> patch, uint i : SV_OutputControlPointID, uint pI : SV_PrimitiveID)
{
    HS_OUTPUT output;
    output.pos = patch[i].pos;
    output.pos.y += 5.0f;

    return output;
}