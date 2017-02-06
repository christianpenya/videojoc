#include "globals.fxh"

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	//float2 UV : TEXCOORD0;
};


PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos = mul( float4(IN.Pos, 1.0), m_World );
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.Normal=normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	return l_Output;
}
