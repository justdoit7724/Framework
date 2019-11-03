
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

cbuffer CB_HV : register(b0)
{
    float isHorizontal;
}

Texture2D aoNDMap : register(t0);
Texture2D aoMap : register(t1);
SamplerState samp : register(s0);

float4 main(PS_INPUT input):SV_Target
{
    const int blurCenter = 5;
    const int blurRad = 5;
    float weight[11] =
    {
        0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f
    };

    const int centerIdx = 5;
    float texelLength = 1 / 760.0f;
    float2 texOffset = float2(isHorizontal * texelLength, (1 - isHorizontal) * texelLength);
    float2 texStart = -blurRad * texelLength;
    
    float4 totalAcess = weight[centerIdx] * aoMap.SampleLevel(samp, input.tex, 0);
    float totalWeight = weight[centerIdx];
    float4 centerND = aoNDMap.SampleLevel(samp, input.tex, 0);
    
    for (int i = 0; i < blurCenter; ++i)
    {
        float2 tex = input.tex + i * texOffset + texStart;
        float4 curND = aoNDMap.SampleLevel(samp, tex, 0);

        if (dot(curND.xyz, centerND.xyz) >= 0.8 &&
            abs(curND.a - centerND.a) <= 0.2)
        {
            float curWeight = weight[i];

            totalAcess += curWeight * aoMap.SampleLevel(samp, tex, 0);
            totalWeight += curWeight;
        }
    }
    for (int j = blurCenter+1; j < 11; ++j)
    {
        float2 tex = input.tex + j * texOffset + texStart;
        float4 curND = aoNDMap.SampleLevel(samp, tex, 0);

        if (dot(curND.xyz, centerND.xyz) >= 0.8 &&
            abs(curND.a - centerND.a) <= 0.2)
        {
            float curWeight = weight[j];

            totalAcess += curWeight * aoMap.SampleLevel(samp, tex, 0);
            totalWeight += curWeight;
        }
    }
    
    return totalAcess / totalWeight;
}