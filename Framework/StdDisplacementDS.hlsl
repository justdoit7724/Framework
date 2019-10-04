struct DS_INPUT
{
    float3 wPos : POSITION;
    float3 normal : TEXCOORD0;
    float2 tex : TEXCOORD1;
    float3 tangent : TEXCOORD2;
};
struct DS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD;
    float3 normal : TEXCOORD1;
    float2 tex : TEXCOORD2;
    float3 tangent : TEXCOORD3;
};
struct Patch
{
    float edgeTess[3] : SV_TessFactor;
    float insideTess : SV_InsideTessFactor;
};

[domain("tri")]
DS_OUTPUT main(Patch patch, float3 bary : SV_DomainLocation, const OutputPatch<DS_INPUT,3> tri)
{
    DS_OUTPUT output;

    output.wPos = bary.x * 
}