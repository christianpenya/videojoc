#include "globals.fx"
#include "Samplers.fxh"

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float2 UV :
    TEXCOORD0;
};

static float m_ZBlurActive = m_RawData[0].x; //1.0;
static float m_ZBlurShowDepths = m_RawData[1].x; //1.0;
static float m_ZBlurConstantBlur = m_RawData[2].x; //0.3;
static float m_ZBlurFocalStart = m_RawData[3].x; //0.1; 
static float m_ZBlurFocalEnd = m_RawData[4].x; //0.2; 
static float m_ZBlurEnd = m_RawData[5].x; //0.8;

float4 GetZBlurColor(float Distance, float4 SourceColor, float4 BlurColor)
{
    float l_Blur=1.0;

    if(Distance < m_ZBlurFocalStart)
        l_Blur=max(Distance / m_ZBlurFocalStart, m_ZBlurConstantBlur);
    else if(Distance > m_ZBlurFocalEnd)
        l_Blur=max(1.0-(Distance-m_ZBlurFocalEnd)/m_ZBlurEnd, m_ZBlurConstantBlur);

    if(l_Blur<1.0)
	{
        return BlurColor*(1-l_Blur)+l_Blur*SourceColor;
    }else
	{	
        return SourceColor;
	}
}

float4 GetZBlurColorDistance(float Distance, float4 SourceColor)
{
	if(Distance<m_ZBlurFocalStart)
		return SourceColor*float4(1,0,0,1);
	else if(Distance<m_ZBlurFocalEnd)
		return SourceColor*float4(0,1,0,1);
	else if(Distance<m_ZBlurEnd)
		return SourceColor*float4(0,0,1,1);
	
	return SourceColor*float4(1,1,0,1);
}

void GetNearFarFromProjectionMatrixB(out float Near, out float Far, float4x4 ProjectionMatrix)
{
	Near=ProjectionMatrix[3].z/ProjectionMatrix[2].z;
	Far=(Near*ProjectionMatrix[2].z/ProjectionMatrix[2].w)/((ProjectionMatrix[2].z/ProjectionMatrix[2].w)-1.0);
}

float4 ZBlurPS(PS_INPUT IN) : SV_Target
{
	float4 l_Color = T0Texture.Sample(S0Sampler, IN.UV);
	if (!m_ZBlurActive)
		return l_Color;
	else
	{
		float4 l_SourceColor=T0Texture.Sample(S0Sampler, IN.UV);
		float l_Depth=T2Texture.Sample(S2Sampler, IN.UV).r;
		float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView, m_InverseProjection);
		float3 l_CameraPosition=m_InverseView[3].xyz;
		float l_Distance=length(l_WorldPosition-l_CameraPosition);
		if(l_Depth==0.0)
		{
			//return float4(1,0,0,1);
			float l_Near;
			GetNearFarFromProjectionMatrixB(l_Near, l_Distance, m_Projection);
		}
		if(m_ZBlurShowDepths)
		{
			//return float4(0,1,0,1);
			return GetZBlurColorDistance(l_Distance, l_SourceColor);
		}else
		{
			//return float4(0,0,1,1);
			return GetZBlurColor(l_Distance, l_SourceColor, T1Texture.Sample(S1Sampler,IN.UV));
		}	
	}
	
	
	
}
