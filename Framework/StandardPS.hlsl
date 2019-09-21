
#define LIGHT_ENABLED 1
#define LIGHT_DISABLED 0
#define LIGHT_MAX_EACH 10

cbuffer DIRECTIONAL_LIGHT : register(b0)
{
    float4 d_Ambient[LIGHT_MAX_EACH];
    float4 d_Diffuse[LIGHT_MAX_EACH];
    float4 d_Specular[LIGHT_MAX_EACH];
    float4 d_Dir[LIGHT_MAX_EACH];
    float4 d_Enabled[LIGHT_MAX_EACH];
};
cbuffer POINT_LIGHT : register(b1)
{
    float4 p_Ambient[LIGHT_MAX_EACH];
    float4 p_Diffuse[LIGHT_MAX_EACH];
    float4 p_Specular[LIGHT_MAX_EACH];
    float4 p_Pos[LIGHT_MAX_EACH];
    float4 p_Range[LIGHT_MAX_EACH];
    float4 p_Att[LIGHT_MAX_EACH];
    float4 p_Enabled[LIGHT_MAX_EACH];
};
cbuffer SPOT_LIGHT : register(b2)
{
    float4 s_Ambient[LIGHT_MAX_EACH];
    float4 s_Diffuse[LIGHT_MAX_EACH];
    float4 s_Specular[LIGHT_MAX_EACH];
    float4 s_Pos[LIGHT_MAX_EACH];
    float4 s_Range[LIGHT_MAX_EACH];
    float4 s_Dir[LIGHT_MAX_EACH];
    float4 s_Spot[LIGHT_MAX_EACH];
    float4 s_Att[LIGHT_MAX_EACH];
    float4 s_Enabled[LIGHT_MAX_EACH];
};
cbuffer EYE : register(b3)
{
    float3 eyePos;
};

cbuffer MATERIAL : register(b4)
{
    float4 mDiffuse;
    float4 mAmbient;
    float4 mSpecular;
    float4 mReflection;
};

Texture2DArray bodyTex : register(t0);
SamplerState bodySampleState : register(s0);

void ComputeDirectionalLight(float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < LIGHT_MAX_EACH; ++i)
    {
        if (d_Enabled[i].x == LIGHT_DISABLED)
            continue;

        ambient += mAmbient * d_Ambient[i];
        float diffuseFactor = dot(-d_Dir[i].xyz, normal);
    
       
        [flatten]
        if (diffuseFactor > 0.0f)
        {
            float3 v = reflect(d_Dir[i].xyz, normal);
            float specFactor = pow(saturate(dot(v, toEye)), mSpecular.w);
            diffuse += diffuseFactor * mDiffuse * d_Diffuse[i];
            spec += specFactor * mSpecular * d_Specular[i];
        }
    }
}
void ComputePointLight(float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec)
{ 
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f); 
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f); 
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < LIGHT_MAX_EACH; ++i)
    {
        if (p_Enabled[i].x == LIGHT_DISABLED)
            continue;
        
        float4 tmpA = 0;
        float4 tmpD = 0;
        float4 tmpS = 0;

        float3 lightVec = p_Pos[i].xyz - pos;
        float d = length(lightVec);
        if (d > p_Range[i].x)
            continue;
        lightVec /= d; // normalize
        tmpA = mAmbient * p_Ambient[i];
        float diffuseFactor = dot(lightVec, normal);

        //flatten for avoiding dynamic branch
        [flatten]
        if (diffuseFactor > 0.0f)
        {
            float3 v = reflect(-lightVec, normal); 
            float specFactor = pow(saturate(dot(v, toEye)), mSpecular.w);
            float att = 1.0f / dot(p_Att[i].xyz, float3(1.0f, d, d * d));
            tmpD = diffuseFactor * mDiffuse * p_Diffuse[i] * att;
            tmpS = specFactor * mSpecular * p_Specular[i] * att;
        }

        ambient += tmpA;
        diffuse += tmpD;
        spec += tmpS;
    }
}
void ComputeSpotLight(float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec)
{ 
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f); 
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f); 
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f); 
    
    for (int i = 0; i < LIGHT_MAX_EACH; ++i)
    {
        if (s_Enabled[i].x == LIGHT_DISABLED)
            continue;
        
        float4 tmpA = 0;
        float4 tmpD = 0;
        float4 tmpS = 0;

        float3 lightVec = s_Pos[i].xyz - pos;
        float d = length(lightVec);

        if (d > s_Range[i].x)
            continue;
    
        lightVec /= d;
        float diffuseFactor = dot(lightVec, normal);

        float spot = 1;
        [flatten]
        if (diffuseFactor > 0.0f)
        {
            float3 v = reflect(-lightVec, normal);
            float specFactor = pow(saturate(dot(v, toEye)), mSpecular.w);
            spot = pow(saturate(dot(-lightVec, s_Dir[i].xyz)), s_Spot[i].x);
            float att = spot / dot(s_Att[i].xyz, float3(1.0f, d, d * d));
            tmpD = diffuseFactor * mDiffuse * s_Diffuse[i] * att;
            tmpS = specFactor * mSpecular * s_Specular[i] * att;
            tmpA = mAmbient * s_Ambient[i] * spot;
        }
        
        ambient += tmpA;
        diffuse += tmpD;
        spec += tmpS;
    }
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 wPos : TEXCOORD;
    float3 normal : TEXCOORD2;
    float2 tex : TEXCOORD3;
};
float4 main(PS_INPUT input) : SV_Target
{
    //uint width, height, spriteCount;
    //bodyTex.GetDimensions(width, height, spriteCount);

    float4 tex = bodyTex.Sample(bodySampleState, float3(input.tex, 0));
    
    input.normal = normalize(input.normal);
    float3 toEye = normalize(eyePos - input.wPos);
    float4 ambient = 0;
    float4 diffuse = 0;
    float4 specular = 0;
    float4 A, D, S;
    ComputeDirectionalLight(input.normal, toEye, A, D, S);
    ambient += A;
    diffuse += D;
    specular += S;
    ComputePointLight(input.wPos, input.normal, toEye, A, D, S);
    ambient += A;
    diffuse += D;
    specular += S;
    ComputeSpotLight(input.wPos, input.normal, toEye, A, D, S);
    ambient += A;
    diffuse += D;
    specular += S;

    ambient *= tex;
    diffuse *= tex;
    
    float4 color = ambient + diffuse + specular;
    color.w = mDiffuse.a;
    
    return color;
}