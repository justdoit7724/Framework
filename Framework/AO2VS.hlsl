struct VS_INPUT
{
    float3 pPos : POSITION;
    float2 tex : TEXCOORD;
};
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 vFarPlanePos : TEXCOORD0;
    float2 tex : TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = float4(input.pPos, 1);
    output.vFarPlanePos = input.pPos;
    output.tex = input.tex;
    return output;
}