struct DS_INPUT
{
    float3 wPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 tex : TEXCOORD2;
    float3 tangent : TEXCOORD3;
};
struct DS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 pPos : TEXCOORD0;
    float3 wPos : TEXCOORD1;
    float3 normal : TEXCOORD2;
    float2 tex : TEXCOORD3;
    float3 tangent : TEXCOORD4;
};
struct Patch
{
    float edgeTess[3] : SV_TessFactor;
    float insideTess : SV_InsideTessFactor;
};

cbuffer CB_TRANSFORMATION : register(b0)
{
    float4x4 vp_mat;
}
cbuffer CB_DISPLACEMENT_SCALE : register(b1)
{
    float dp_scale;
}

Texture2DArray bump_tex : register(t0);
SamplerState samp : register(s0);

[domain("tri")]
DS_OUTPUT main(Patch patch, float3 bary : SV_DomainLocation, const OutputPatch<DS_INPUT,3> tri)
{
    DS_OUTPUT output;

    output.wPos =
        bary.x * tri[0].wPos +
        bary.y * tri[1].wPos +
        bary.z * tri[2].wPos;
    output.normal = normalize(
        bary.x * tri[0].normal +
        bary.y * tri[1].normal +
        bary.z * tri[2].normal);
    output.tangent = normalize(
        bary.x * tri[0].tangent +
        bary.y * tri[1].tangent +
        bary.z * tri[2].tangent);
    output.tex =
        bary.x * tri[0].tex +
        bary.y * tri[1].tex +
        bary.z * tri[2].tex;

    float h = bump_tex.SampleLevel(samp, float3(output.tex, 0), 0).x;
    output.wPos += output.normal * (h - 1) * dp_scale;
    output.pPos = output.pos = mul(vp_mat, float4(output.wPos, 1));

    return output;
}