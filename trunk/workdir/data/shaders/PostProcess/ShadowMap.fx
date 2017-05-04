#include "globals.fx"

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Depth : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT IN)
{
	PS_INPUT l_Out=(PS_INPUT)0;
	l_Out.Pos=mul(float4(IN.Pos.xyz, 1.0), m_World);
	l_Out.Pos=mul(l_Out.Pos, m_View);
	l_Out.Pos=mul(l_Out.Pos, m_Projection);
	l_Out.Depth=l_Out.Pos;
	
	return l_Out;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	return(float4(0.0, 1.0, 1.0, 1.0));
	float l_Depth=IN.Depth.z/IN.Depth.w;
	return float4(l_Depth, l_Depth, l_Depth, 1);
}
