
cbuffer CB_PROPERTY : register(b0)
{
    float4x4 vp_mat;
}
cbuffer CB_EYE : register(b1)
{
    float3 eye_pos;
}

struct GS_INPUT
{
    float3 pos : TEXCOORD0;
    float2 size : TEXCOORD1;
};
struct GS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

[maxvertexcount(4)]
void main(point GS_INPUT p[1], inout TriangleStream<GS_OUTPUT> stream)
{
    float3 forward = normalize(float3(
    eye_pos.x - p[0].pos.x,
    0,
    eye_pos.z - p[0].pos.z));
    float3 up = float3(0, 1, 0);
    float3 right = cross(up, forward);

    float2 hSize = p[0].size * 0.5f;

    GS_OUTPUT output[4];
    output[0].pos = mul(vp_mat, float4(p[0].pos - right * hSize.x + up * hSize.y, 1));
    output[0].tex = float2(0, 0);
    output[1].pos = mul(vp_mat, float4(p[0].pos - right * hSize.x - up * hSize.y, 1));
    output[1].tex = float2(0, 1);
    output[2].pos = mul(vp_mat, float4(p[0].pos + right * hSize.x + up * hSize.y, 1));
    output[2].tex = float2(1, 0);
    output[3].pos = mul(vp_mat, float4(p[0].pos + right * hSize.x - up * hSize.y, 1));
    output[3].tex = float2(1, 1);

    stream.Append(output[0]);
    stream.Append(output[1]);
    stream.Append(output[2]);
    stream.Append(output[3]);
}