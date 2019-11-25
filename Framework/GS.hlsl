
cbuffer CB_VS_PROPERTY : register(b0)
{
    float4x4 w_mat;
    float4x4 vp_mat;
    float4x4 n_mat;
}

struct GS_INPUT
{
    float3 lPos : POSITION;
    float3 n : NORMAL;
    float2 tex : TEXCOORD;
};
struct GS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD0;
    float3 n : TEXCOORD1;
    float2 tex : TEXCOORD2;
};

void Subdivide(GS_INPUT inVertice[3], out GS_INPUT outVertice[6])
{
    GS_INPUT m[3];
    m[0].lPos = (inVertice[0].lPos + inVertice[1].lPos) * 0.5f;
    m[1].lPos = (inVertice[1].lPos + inVertice[2].lPos) * 0.5f;
    m[2].lPos = (inVertice[2].lPos + inVertice[0].lPos) * 0.5f;
    m[0].lPos = m[0].n = normalize(m[0].lPos);
    m[1].lPos = m[1].n = normalize(m[1].lPos);
    m[2].lPos = m[2].n = normalize(m[2].lPos);

    m[0].tex = (inVertice[0].tex + inVertice[1].tex) * 0.5f;
    m[1].tex = (inVertice[1].tex + inVertice[2].tex) * 0.5f;
    m[2].tex = (inVertice[2].tex + inVertice[0].tex) * 0.5f;
   
    outVertice[0] = inVertice[0];
    outVertice[1] = m[0];
    outVertice[2] = m[2];
    outVertice[3] = m[1];
    outVertice[4] = inVertice[2];
    outVertice[5] = inVertice[1];
}

[maxvertexcount(32)]
void main(triangle GS_INPUT gin[3], inout TriangleStream<GS_OUTPUT> triStream)
{
    GS_INPUT vertice[6];
    Subdivide(gin, vertice);

    GS_OUTPUT outVertice[6];

    [unroll]
    for (int i = 0; i < 6; ++i)
    {
        outVertice[i].wPos = mul(w_mat, float4(vertice[i].lPos, 1)).xyz;
        outVertice[i].pos = mul(vp_mat, float4(outVertice[i].wPos, 1));
        outVertice[i].n = mul(n_mat, float4(vertice[i].n, 1)).xyz;
        outVertice[i].tex = vertice[i].tex;
    }
    
    [unroll]
    for (int j = 0; j < 5; ++j)
    {
        triStream.Append(outVertice[j]);
    }
    triStream.RestartStrip();

    triStream.Append(outVertice[1]);
    triStream.Append(outVertice[5]);
    triStream.Append(outVertice[3]);

}