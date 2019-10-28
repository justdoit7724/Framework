struct VS_INPUT
{
    float3 pos : POSITION;
    float vFarPlaneCornerIdx : CORNER_IDX;
    float2 tex : TEXCOORD;
};
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 vFarPlane : TEXCOORD0;
    float2 tex : TEXCOORD1;
};

cbuffer CB_AO_CORNER : register(b0)
{
    float4 vFarPlaneCorner[4];
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = float4(input.pos, 1);
    output.vFarPlane = vFarPlaneCorner[input.vFarPlaneCornerIdx].xyz;
    output.tex = input.tex;
    return output;
}