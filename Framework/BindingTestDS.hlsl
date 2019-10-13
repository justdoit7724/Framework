struct Patch
{
    float edgeTessFactor[3] : SV_TessFactor;
    float inTessFactor : SV_InsideTessFactor;
};

struct DS_INPUT
{
    float3 pos : TEXCOORD0;
};
struct DS_OUTPUT
{
    float3 pos : TEXCOORD0;
};

[domain("tri")]
DS_OUTPUT main(Patch p, float3 uv : SV_DomainLocation, const OutputPatch<DS_INPUT,3> input)
{
    DS_OUTPUT output;
    output.pos = uv;

    return output;
}