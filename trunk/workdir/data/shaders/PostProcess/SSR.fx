#include "Samplers.fxh"
#include "globals.fx"

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float2 UV :
    TEXCOORD0;
};

static bool m_Enabled=m_RawData[0].x;
static float m_SSROpacity=m_RawData[1].x;
static float m_OffsetScreen=m_RawData[2].x;
static float2 m_ScreenResolution=float2(m_RawData[3].x, m_RawData[4].x);

float3 CalcIntersection(float3 PositionA, float3 VDirA, float3 PositionB, float3 VDirB)
{
	//line vs plane method
	float3 l_Normal=normalize(cross(VDirA, VDirB));
	l_Normal=normalize(cross(l_Normal, VDirB));
	float d=dot((PositionB-PositionA), l_Normal)/(dot(VDirA, l_Normal));
	return PositionA+d*VDirA;
}

float3 GetReflectedColor(float3 WorldPosition, float3 Nn, float2 UV)
{
	float3 l_CameraToWorldPosition=WorldPosition-m_InverseView[3].xyz;
	float l_CameraToWorldDistance=length(l_CameraToWorldPosition);
	float3 l_CameraToWorldNormalized=normalize(l_CameraToWorldPosition);
	float3 l_ReflectedVector=normalize(reflect(l_CameraToWorldNormalized, Nn));
	float3 l_WorldPositionWithReflection=WorldPosition+l_ReflectedVector;
	float4 l_WorldPositionWithReflectionUV=mul(float4(l_WorldPositionWithReflection, 1.0), m_View);
	l_WorldPositionWithReflectionUV=mul(float4(l_WorldPositionWithReflectionUV.xyz, 1.0), m_Projection);
	float2 l_ReflectedDirectionUV = 0.5 * float2(l_WorldPositionWithReflectionUV.x/l_WorldPositionWithReflectionUV.w,- l_WorldPositionWithReflectionUV.y/l_WorldPositionWithReflectionUV.w) + 0.5; 
	l_ReflectedDirectionUV=normalize(l_ReflectedDirectionUV-UV);
	float3 l_Color;
	float2 l_ScreenInverted=float2(1.0/m_ScreenResolution.x, 1.0/m_ScreenResolution.y);
	float2 l_UV=UV;
	float i=0;
	float l_Increment=1.0/50.0;

	do
	{
		i=i+l_Increment;
		l_UV=UV+l_ReflectedDirectionUV*i;
		float l_Depth=T2Texture.Sample(S2Sampler, l_UV).x;
		float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, l_UV, m_InverseView, m_InverseProjection);
		float3 l_PixelVector=normalize(l_WorldPosition-WorldPosition);
		//Buscar punto de corte entre el Vector reflejado y el vector que une el ojo con el punto de de pantalla en coordenadas de mundo
		float3 l_EyeToWorldPosition=l_WorldPosition-m_InverseView[3].xyz;
		float l_DistanceToWorldPosition=length(l_EyeToWorldPosition);
		float3 l_EyeToWorldPositionVector=l_EyeToWorldPosition/l_DistanceToWorldPosition;
		float3 l_IntersectionPoint=CalcIntersection(m_InverseView[3].xyz, l_EyeToWorldPositionVector, WorldPosition, l_ReflectedVector);
		float l_DistanceToIntersectionPoint=length(l_IntersectionPoint - m_InverseView[3].xyz);
		if(l_DistanceToIntersectionPoint>=l_DistanceToWorldPosition)
			break;
	} while(i<1.0);
	
	if (l_UV.x>=(1.0-m_OffsetScreen) || l_UV.x<m_OffsetScreen || l_UV.y>=(1.0-m_OffsetScreen) || l_UV.y<m_OffsetScreen)
		l_UV=UV;
	else
	{
		bool l_CorrectJitter=true;
		if(l_CorrectJitter)
		{
			float l_TotalElements=30.0;
			l_Increment=l_Increment/l_TotalElements;
			i=0;
			float2 l_UV2=l_UV;
			do
			{
				i=i+1;
				l_UV=l_UV2-l_ReflectedDirectionUV*i*l_Increment;
				float l_Depth=T2Texture.Sample(S2Sampler, l_UV).x;
				float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, l_UV, m_InverseView, m_InverseProjection);
				float3 l_PixelVector=normalize(l_WorldPosition-WorldPosition);
				//Buscar punto de corte entre el Vector reflejado y el vector que une el ojo con el punto de de pantalla en coordenadas de mundo
				float3 l_EyeToWorldPosition=l_WorldPosition - m_InverseView[3].xyz;
				float l_DistanceToWorldPosition=length(l_EyeToWorldPosition);
				float3 l_EyeToWorldPositionVector=l_EyeToWorldPosition/l_DistanceToWorldPosition;
				float3 l_IntersectionPoint=CalcIntersection(m_InverseView[3].xyz, l_EyeToWorldPositionVector, WorldPosition, l_ReflectedVector);
				float l_DistanceToIntersectionPoint=length(l_IntersectionPoint - m_InverseView[3].xyz);
				if(l_DistanceToIntersectionPoint<l_DistanceToWorldPosition)
				{
					l_UV=l_UV+l_ReflectedDirectionUV*l_Increment;
					break;
				}
			} while(i<l_TotalElements);
		}
	}
	l_Color=T1Texture.Sample(S1Sampler, l_UV).xyz;
	return l_Color;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	float l_Pct=T0Texture.Sample(S0Sampler, IN.UV).x;
	if(l_Pct>0.9 && m_Enabled)
	{
		float l_Depth=T2Texture.Sample(S2Sampler, IN.UV.xy).x;
		float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView, m_InverseProjection);
		float3 Nn=normalize(T3Texture.Sample(S3Sampler, IN.UV.xy).xyz * 2.0 - 1.0);
		return float4(GetReflectedColor(l_WorldPosition, Nn, IN.UV), m_SSROpacity);
	}
	clip(-1);
	return float4(1,1,1,1);
}