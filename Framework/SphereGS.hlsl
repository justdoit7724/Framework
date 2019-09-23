

cbuffer CB_VS_PROPERTY : register(b0)
{
    float4x4 wvp_mat;
};


struct GS_INPUT
{
    float3 pos : TEXCOORD0;
};
struct GS_OUTPUT
{
    float4 pos : SV_POSITION;
};

[maxvertexcount(8)]
GS_OUTPUT main(triangle GS_INPUT p[3], inout TriangleStream<GS_OUTPUT> stream)
{
    //         v0       
	//        / \       
	//    m2 *---* m0
	//      / \ / \     
	//    v2---*---v1   
	//        m1    

    float3 v0 = p[0].pos;
    float3 v1 = p[1].pos;
    float3 v2 = p[2].pos;
    float3 m0 = normalize((p[0].pos + p[1].pos) * 0.5f);
    float3 m1 = normalize((p[1].pos + p[2].pos) * 0.5f);
    float3 m2 = normalize((p[2].pos + p[0].pos) * 0.5f);

    GS_OUTPUT output[8];
    output[0].pos = mul(wvp_mat, float4(v0, 1));
    output[0].pos = mul(wvp_mat, float4(v0, 1));
    output[0].pos = mul(wvp_mat, float4(v0, 1));
    output[0].pos = mul(wvp_mat, float4(v0, 1));
    output[0].pos = mul(wvp_mat, float4(v0, 1));
    output[0].pos = mul(wvp_mat, float4(v0, 1));
    output[0].pos = mul(wvp_mat, float4(v0, 1));
    output[0].pos = mul(wvp_mat, float4(v0, 1));

}