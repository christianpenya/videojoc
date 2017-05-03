#include "Globals.fxh"
#include "Samplers.fxh"

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float3 WorldPosition: TEXCOORD1;
};

PS_INPUT VS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos = mul(float4(IN.Pos.xyz, 1.0), m_World);
	l_Output.WorldPosition = l_Output.Pos.xyz;
	l_Output.Pos = mul(l_Output.Pos, m_View);
	l_Output.Pos = mul(l_Output.Pos, m_Projection);
	l_Output.Normal = normalize(mul(IN.Normal, (float3x3)m_World));
	
	return l_Output;
}

float4 PS(PS_INPUT IN) : SV_Target
{	
	float3 l_Normal=normalize(IN.Normal);
	float3 l_EyeToWorldPosition = normalize(IN.WorldPosition-m_CameraPosition.xyz);
	float3 l_ReflectVector = normalize(reflect(l_EyeToWorldPosition, l_Normal));
	float3 l_ReflectColor = T0Texture.Sample(S0Sampler, l_ReflectVector).xyz;
	return float4(l_ReflectColor,1);
}