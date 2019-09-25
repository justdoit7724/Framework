struct HS_INPUT
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

cbuffer CB_EYE : register(b0)
{
    float3 eye_pos;
}

Patch ConstantHS(InputPatch<HS_INPUT, 4> patch, uint pI : SV_PrimitiveID)
{
    float3 center = (patch[0].pos + patch[1].pos + patch[2].pos + patch[3].pos)*0.25f;
    float dist = length(center - eye_pos);

    const float d0 = 40.0f;
    const float d1 = 200.0f;
    float tess = 64.0f * saturate((d1 - dist) / (d1 - d0));

    Patch p;
    p.edgeTessFactor[0] = tess;
    p.edgeTessFactor[1] = tess;
    p.edgeTessFactor[2] = tess;
    p.edgeTessFactor[3] = tess;
    p.inTessFactor[0] = tess;
    p.inTessFactor[1] = tess;

    return p;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HS_OUTPUT main(InputPatch<HS_INPUT, 4> patch, uint i : SV_OutputControlPointID, uint pI : SV_PrimitiveID)
{
    HS_OUTPUT output;
    output.pos = patch[i].pos;

    return output;
}