#include "globals.fx"

Texture2D DiffuseTexture :
register(t0);
SamplerState LinearSampler :
register(s0);

struct VS_INPUT
{
float3 Pos :
    POSITION;
float3 Normal :
    NORMAL;
float2 UV :
    TEXCOORD0;
};

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float3 Normal :
    NORMAL;
float2 UV :
    TEXCOORD0;

};

PS_INPUT VS( VS_INPUT IN )
{
PS_INPUT l_Output = (PS_INPUT)0;
  
l_Output.Normal=normalizenormalize(IN.Normal).xyz;

l_Output.Pos = float4( IN.Pos.xyz, 1.0 );
l_Output.UV = IN.UV;

    return l_Output;
}

float4 PS( PS_INPUT IN ) : SV_Target
{
    float3 NormalPixel = normalize(IN.Normal);
    //float3 WorldPos = IN.WorldPosition;
    float4 ColorPixel = DiffuseTexture.Sample(LinearSampler, IN.UV) * float4(m_RawData[0].xyzw);
   // float3 l_LAmbient = m_LightAmbient.xyz * ColorPixel.xyz;

 
    return float4(ColorPixel, 1.0);
}
