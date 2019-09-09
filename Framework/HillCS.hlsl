
cbuffer CB_HEIGHT_INFO : register(b0)
{
    float2 range;
};
cbuffer CB_HILL_RESOLUTION : register(b1)
{
    float2 hillResol;
};

Texture2D<float4> hillTexture : register(t0);

RWTexture2D<float> posBuffer : register(u0);


[numthreads(16, 16, 1)]
void main(int3 id : SV_DispatchThreadID)
{ 
    if(id.x >= hillResol.x || id.y >= hillResol.y)
        return;

    uint texWN, texHN;
    hillTexture.GetDimensions(texWN, texHN);

    float2 ratio = float2((texWN - 1) / (hillResol.x - 1), (texHN - 1) / (hillResol.y - 1));
    int2 texIdx = ceil(id.xy * ratio);

    posBuffer[id.xy] = lerp(range.x, range.y, hillTexture[texIdx.xy].y);

}