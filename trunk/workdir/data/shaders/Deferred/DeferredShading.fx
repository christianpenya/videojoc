#include "globals.fx"

static const float Pi = 3.14159265359;


Texture2D T0Texture :
register(t0);
SamplerState S0Sampler :
register(s0);

Texture2D T1Texture :
register(t1);
SamplerState S1Sampler :
register(s1);


Texture2D T2Texture :
register(t2);
SamplerState S2Sampler :
register(s2);

struct VS_INPUT
{
float3 Pos :
    POSITION;
float2 UV :
    TEXCOORD0;
};

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float2 UV :
    TEXCOORD0;
};

struct PixelOutputType
{
float4 Target0 : SV_Target0;
float4 Target1 : SV_Target1;
};

PS_INPUT VS(VS_INPUT IN)
{
    PS_INPUT l_Output = (PS_INPUT)0;
    l_Output.Pos=float4(IN.Pos, 1.0);
    l_Output.UV=IN.UV;
    return l_Output;
}

PixelOutputType PS(PS_INPUT IN) : SV_Target
{
	
	
	float4 l_DiffuseColor =T0Texture.Sample(S0Sampler, IN.UV);

	float3 l_Normal = Texture2Normal(T1Texture.Sample(S1Sampler, IN.UV));

	float l_Depth=T2Texture.Sample(S2Sampler, IN.UV).r;

	float3 l_WorldPos=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView,
m_InverseProjection);
	

	float3 l_Diffuse = float3(0.0, 0.0, 0.0);
    float3 l_Specular = float3(0.0, 0.0, 0.0);

   	
    CalculateSingleLight(0, l_Normal, l_WorldPos, l_DiffuseColor, l_Diffuse, l_Specular);


    return float4(l_Diffuse+l_Specular, 1.0);
   

}
