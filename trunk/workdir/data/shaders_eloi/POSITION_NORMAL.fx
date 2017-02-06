//POSITION_NORMAL_TEXTURE_SHADER.FX
#include "globals.fxh"

Texture2D DiffuseTexture : register( t0 );
SamplerState LinearSampler : register( s0 );

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	//float2 UV : TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : POSITION;
	float4 Normal : NORMAL;
	//float2 UV : TEXCOORD0;
	//float3 WorldNormal : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------


PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos = mul( float4(IN.Pos, 1.0), m_World );
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.Normal=normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	return l_Output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT IN) : SV_Target
{
	return float4(IN.Normal.xyz, 1.0);
}