#include "Colors.fxh"
#include "globals.fxh"



struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	//float2 UV : TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	//float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
};


PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	float4 lPos = float4( IN.Pos.xyz, 1.0 );
	l_Output.Pos = mul( lPos, m_World );
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );

	l_Output.Normal=normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	l_Output.WorldNormal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	return l_Output;
}

float4 PS( PS_INPUT IN ) : SV_Target
{






	return float4(normalize(mPS_LightDirection[1].xyz),1.0);
	return float4(IN.Normal.xyz, 1.0);
	return Red();
}

