#include "globals.fx"
#include "Samplers.fxh"

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

PS_INPUT VS(VS_INPUT IN)
{
    PS_INPUT l_Output = (PS_INPUT)0;
    l_Output.Pos=float4(IN.Pos, 1.0);
    l_Output.UV=IN.UV;
    return l_Output;
}

float4 CalculateShadow(float3 l_WorldPos)
{
	float4 l_Shadow = float4(1,1,1,1);
	
	if(m_UseShadowMapArray[0])
	{
		float4 l_LightViewPosition = mul(float4(l_WorldPos, 1), m_LightView[0]);
		l_LightViewPosition = mul(l_LightViewPosition, m_LightProjection[0]);
		float2 l_ProjectedLightCoords = float2(((l_LightViewPosition.x/l_LightViewPosition.w)/2.0f)+0.5f, ((-l_LightViewPosition.y/l_LightViewPosition.w)/2.0f)+0.5f);
		
		float l_DepthShadowMap=T4Texture.Sample(S4Sampler, l_ProjectedLightCoords).r;
		
		float l_LightDepth=l_LightViewPosition.z/l_LightViewPosition.w;
		float m_ShadowMapBias = 0.001f;
		l_DepthShadowMap=l_DepthShadowMap+m_ShadowMapBias;
		
		if((saturate(l_ProjectedLightCoords.x)==l_ProjectedLightCoords.x) && (saturate(l_ProjectedLightCoords.y)==l_ProjectedLightCoords.y))
		{
			//l_Shadow = float4(l_ProjectedLightCoords.x,0,0,1);
			if(l_LightDepth>l_DepthShadowMap)
			{
				l_Shadow = float4(0,0,0,1);
			}
			//l_Shadow =float4(1,1,1,1);
		}
		/*else		
			l_Shadow =float4(0,0,0,1);*/
	}
	
	return l_Shadow;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	float4 l_DiffuseColor =T0Texture.Sample(S0Sampler, IN.UV);
	float3 l_Normal = Texture2Normal(T1Texture.Sample(S1Sampler, IN.UV));

	float l_Depth=T2Texture.Sample(S2Sampler, IN.UV).r;

	float3 l_WorldPos=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView, m_InverseProjection);

	float3 l_Diffuse = float3(0.0, 0.0, 0.0);
    float3 l_Specular = float3(0.0, 0.0, 0.0);
   	
    CalculateSingleLight(0, l_Normal, l_WorldPos, l_DiffuseColor, l_Diffuse, l_Specular);
	//return float4(CalculateShadow(l_WorldPos), 0.0, 0.0, 1.0);
    return float4(l_Diffuse+l_Specular, 1.0) * CalculateShadow(l_WorldPos);
}
