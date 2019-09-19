
Texture2D image;
sampler samp;

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 size : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_Target
{
    return image.Sample(samp, input.size);
}