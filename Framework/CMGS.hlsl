
struct GS_INPUT
{
    float3 wPos : TEXCOORD0;
    float2 tex : TEXCOORD1;
};
struct GS_OUTPUT
{
    float4 pos : SV_POSITION;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

cbuffer CB_TRANSFORM : register(b0)
{
    float4x4 v_mat[6];
    float4x4 p_mat;
}

[maxvertexcount(18)]
void main(triangle GS_INPUT input[3], inout TriangleStream<GS_OUTPUT> stream)
{
    for (int f = 0; f < 6; ++f)
    {
        GS_OUTPUT output;
        output.RTIndex = f;

        for (int v = 0; v < 3; v++)
        {
            float4x4 vp_mat = mul(p_mat, v_mat[f]);
            output.pos = mul(vp_mat, float4(input[v].wPos, 1));

            stream.Append(output);
        }

        stream.RestartStrip();

    }

}