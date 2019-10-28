struct VS_INPUT
{
    float3 pos : POSITION;
    float farPlaneCornerIdx : CORNER_IDX;
    float2 tex : TEXCOORD;
};
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 farPlane : TEXCOORD0;
    float2 tex : TEXCOORD1;
};

cbuffer CB_TRANSF : register(b0)
{
    float4x4 wvp;
}
cbuffer CB_AO_CORNER : register(b1)
{
    float4 farPlaneCorner[4];
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(wvp, float4(input.pos, 1)).xyz;
    output.farPlane = farPlaneCorner[input.farPlaneCornerIdx].xyz;
    output.tex = input.tex;
    return output;
}