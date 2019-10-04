struct Patch
{
    float edgeTess[3] : SV_TessFactor;
    float insideTess : SV_InsideTessFactor;
};
struct HS_INPUT
{
    float3 wPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 tex : TEXCOORD2;
    float3 tangent : TEXCOORD3;
    float tessFactor : TEXCOORD4;
};
struct HS_OUTPUT
{
    float3 wPos : POSITION;
    float3 normal : TEXCOORD0;
    float2 tex : TEXCOORD1;
    float3 tangent : TEXCOORD2;
};

Patch ConstantHS(InputPatch<HS_INPUT,3> input, uint patchID : SV_PrimitiveID)
{
    Patch output;
    output.edgeTess[0] = 0.5f * (input[1].tessFactor + input[2].tessFactor);
    output.edgeTess[1] = 0.5f * (input[2].tessFactor + input[0].tessFactor);
    output.edgeTess[2] = 0.5f * (input[0].tessFactor + input[1].tessFactor);

    output.insideTess = output.edgeTess[0];
    return output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
HS_OUTPUT main(
    InputPatch<HS_INPUT,3> input, 
    uint id : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HS_OUTPUT output;
    output.wPos = input[id].wPos;
    output.normal = input[id].normal;
    output.tex = input[id].tex;
    output.tangent = input[id].tangent;

    return output;
}

