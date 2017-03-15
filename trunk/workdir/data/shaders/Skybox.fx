#include "globals.fx"
#include "samplers.fxh"

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 UV : TEXCOORD0;
};


void GetNearFarFromProjectionMatrix(out float Near, out float Far, float4x4 ProjectionMatrix)
{
	Near=ProjectionMatrix[3].z/ProjectionMatrix[2].z;
	Far=(Near*ProjectionMatrix[2].z/ProjectionMatrix[2].w)/((ProjectionMatrix[2].z/ProjectionMatrix[2].w)-1.0);
}

PS_INPUT VS(VS_INPUT IN)
{
	PS_INPUT l_Output=(PS_INPUT)0;
	l_Output.UV=IN.Pos.xyz;
	float l_Near;
	float l_Far;
	GetNearFarFromProjectionMatrix(l_Near, l_Far, m_Projection);
	l_Output.Pos=float4(m_CameraPosition+l_Output.UV*l_Far*0.95, 1.0);
	l_Output.Pos=mul(l_Output.Pos, m_ViewProjection);
	return l_Output;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	return T0TextureCube.Sample(S3Sampler, IN.UV);
}
