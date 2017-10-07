#include "globals.fx"
#include "Samplers.fxh"

static const float Pi = 3.14159265359;

struct PixelOutputType
{
	float4 Target0 : SV_Target0;
	float4 Target1 : SV_Target1;
};

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

float3 GetPBRLightDirection(int IdLight, float3 PixelPosition, out float LightDistance)
{
	float3 l_LightDirection=float3(0,0,0);
	if(m_LightTypeArray[IdLight]==0)
	{
		//Omni
		l_LightDirection=PixelPosition-m_LightPosition[IdLight].xyz;
		LightDistance=length(l_LightDirection);
		l_LightDirection/=LightDistance;
	}
	else if(m_LightTypeArray[IdLight]==2)
	{
	//Directional Light
		l_LightDirection=m_LightDirection[IdLight].xyz;
		LightDistance=0;
	}
	else if(m_LightTypeArray[IdLight]==1)
	{
		//Spot
		l_LightDirection=PixelPosition-m_LightPosition[IdLight].xyz;
		LightDistance=length(l_LightDirection);
		l_LightDirection/=LightDistance;
	}
	return l_LightDirection;
}
float CalcPBRLightDiffuse(int IdLight, float3 PixelNormal, float3 LightDirection)
{
	float OutputValue=0.0;
	if(m_LightTypeArray[IdLight]==0)
	{
		//Omni
		OutputValue=saturate(dot(-LightDirection, PixelNormal));
	}
	else if(m_LightTypeArray[IdLight]==2)
	{
		//Directional Light
		OutputValue=saturate(dot(-LightDirection, PixelNormal));
	}
	else if(m_LightTypeArray[IdLight]==1)
	{
		//Spot
		float3 KL=dot(-LightDirection, -m_LightDirection[IdLight].xyz);
		float l_Cos=cos(m_LightFallOffAngleArray[IdLight]*0.5);
		OutputValue=l_Cos<KL ? saturate(dot(PixelNormal, -LightDirection)) : 0.0;
	}
	return OutputValue;
}

float CalcLightAttenuation(int IdLight, float l_LightDistance, float3 l_WorldPosition)
{
	float lightAttenuation=1.0;
	if(m_LightTypeArray[IdLight]==0)
	{
		//Omni
		lightAttenuation = 1 - saturate((l_LightDistance - m_LightAttenuationStartRangeArray[IdLight]) / (m_LightAttenuationEndRangeArray[IdLight] - m_LightAttenuationStartRangeArray[IdLight]));
	}
	else if(m_LightTypeArray[IdLight]==2)
	{
		//Directional Light
		lightAttenuation=1.0;
	}
	else if(m_LightTypeArray[IdLight]==1)
	{
		//Spot
		// Distance attenuation (linear)
		float l_DistanceAttenuation = 1 - saturate((l_LightDistance - m_LightAttenuationStartRangeArray[IdLight]) / (m_LightAttenuationEndRangeArray[IdLight] - m_LightAttenuationStartRangeArray[IdLight]));

		float3 l_RayDirection = normalize(l_WorldPosition - m_LightPosition[IdLight]);
		float l_Distance = distance(l_WorldPosition, m_LightPosition[IdLight]);
		float l_DirectionContrib = dot(l_RayDirection, normalize(m_LightDirection[IdLight]));
		
		// Angle attenuation (linear)
		float l_SpotAttenuation = 1 - saturate((acos(l_DirectionContrib) - m_LightAngleArray[IdLight]/2) / (m_LightFallOffAngleArray[IdLight]/2 - m_LightAngleArray[IdLight]/2));
		lightAttenuation *= l_DistanceAttenuation;
		lightAttenuation *= l_SpotAttenuation;		
	}
	return lightAttenuation;
}


void CalculatePBRSingleLight(int IdLight, float3 BaseColor, float3 WorldPosition, float3 WorldNormal, float3 SpecularColor, float Roughness, out float3 DiffuseContrib, out float3 SpecularContrib)
{
	float l_ShadowContrib=1.0;
	DiffuseContrib=float3(0.0,0.0,0.0);
	SpecularContrib=float3(0.0,0.0,0.0);
	if(m_LightEnabledArray[IdLight]==true)
	{
		float l_LightDistance;
		float3 l_LightDirection=GetPBRLightDirection(IdLight, WorldPosition,l_LightDistance);
		float l_Attenuation=CalcLightAttenuation(IdLight, l_LightDistance, WorldPosition);
		float l_DiffuseContrib=CalcPBRLightDiffuse(IdLight, WorldNormal,l_LightDirection);
		float3 l_Diffuse=l_DiffuseContrib*m_LightColor[IdLight].xyz;
		// SPECULAR
		float3 l_WorldPositionToCamera=normalize(m_InverseView[3].xyz-WorldPosition);
		float3 l_HalfVector=normalize(l_WorldPositionToCamera-l_LightDirection);
		float l_HalfVectorDotNormal=dot(l_HalfVector, WorldNormal);
		float l_NormalDotLight=dot(-l_LightDirection, WorldNormal);
		float l_NormalDotCamera=dot(WorldNormal, l_WorldPositionToCamera);
		// Fresnel term
		float3 l_SchlickFresnel=SpecularColor + (1 - SpecularColor) * (pow(1 - dot(l_WorldPositionToCamera, l_HalfVector), 5) / (6 - 5*(1-Roughness)));
		// Distribution term
		float l_RoughnessPow2=Roughness*Roughness;
		float l_RoughnessPow22=l_RoughnessPow2*l_RoughnessPow2;
		float l_RoughnessPow2Half=l_RoughnessPow2*0.5;
		float l_Denominator=l_HalfVectorDotNormal*l_HalfVectorDotNormal*(l_RoughnessPow22 - 1) + 1;
		float l_GGXDistribution=l_RoughnessPow22/(Pi*l_Denominator*l_Denominator);
		// Geometry term
		float l_SchlickGGXGeometry=(l_NormalDotCamera / (l_NormalDotCamera*(1 - l_RoughnessPow2Half) + l_RoughnessPow2Half));
		//float l_GGXGeometry = (2*l_NormalDotCamera) / (l_NormalDotCamera +sqrt(l_RoughnessPow2 +
		// (1 - l_RoughnessPow2)*pow(l_NormalDotCamera, 2))); // ggxG without schlick approximation
		float3 l_Specular=((l_SchlickFresnel*l_GGXDistribution*l_SchlickGGXGeometry) / 4*l_NormalDotLight*l_NormalDotCamera).xxx * m_LightColor[IdLight].xyz*SpecularColor.rgb;
		
		if(m_UseShadowMapArray[IdLight])
		{
			float4 l_LightViewPosition=mul(float4(WorldPosition, 1),
			m_LightView[IdLight]);
			l_LightViewPosition=mul(l_LightViewPosition,m_LightProjection[IdLight]);
			float2 l_ProjectedLightCoords=float2(((l_LightViewPosition.x/l_LightViewPosition.w)/2.0f)+0.5f,((-l_LightViewPosition.y/l_LightViewPosition.w)/2.0f)+0.5f);
			float l_DepthShadowMap=T4Texture.Sample(S4Sampler,l_ProjectedLightCoords).r;
			float l_LightDepth=l_LightViewPosition.z/l_LightViewPosition.w;
			l_DepthShadowMap=l_DepthShadowMap+m_ShadowMapBiasArray[IdLight];
			if((saturate(l_ProjectedLightCoords.x)==l_ProjectedLightCoords.x) &&(saturate(l_ProjectedLightCoords.y)==l_ProjectedLightCoords.y))
			{
				if(l_LightDepth>l_DepthShadowMap)
					l_ShadowContrib=saturate(1.0f - m_ShadowMapStrengthArray[IdLight]);
			}
			DiffuseContrib = float3(l_DepthShadowMap,0,0);
		}
		
		DiffuseContrib=l_Attenuation*BaseColor*l_Diffuse*m_LightIntensityArray[IdLight]*l_ShadowContrib;
		SpecularContrib=max(float3(0,0,0), l_Attenuation*l_Specular*m_LightIntensityArray[IdLight]*l_ShadowContrib);

	}
}

float2 OctWrap(float2 v)
{
	return (1.0 - abs(v.yx)) * (v.xy >= 0.0 ? 1.0 : -1.0);
}

float3 Decode(float2 encN)
{
	encN = encN * 2.0 - 1.0;
	float3 n;
	n.z = 1.0 - abs(encN.x) - abs(encN.y);
	n.xy = n.z >= 0.0 ? encN.xy : OctWrap(encN.xy);
	n = normalize(n);
	return n;
}

PS_INPUT VS(VS_INPUT IN)
{
    PS_INPUT l_Output = (PS_INPUT)0;
    l_Output.Pos=float4(IN.Pos, 1.0);
    l_Output.UV=IN.UV;
    return l_Output;
}

PixelOutputType PS(PS_INPUT IN) : SV_Target
{
	PixelOutputType l_OUT=(PixelOutputType)0;

	float4 l_BaseColor=T0Texture.Sample(S0Sampler, IN.UV);
	float4 l_RoughnessColor=T4Texture.Sample(S4Sampler, IN.UV);
	float l_Roughness=pow(l_RoughnessColor.a, 0.4);
	l_Roughness=pow(abs(l_Roughness), 2.2);
	float4 l_SpecularColor=float4(lerp(0.04f.xxx, l_BaseColor.xyz, l_BaseColor.w), 1.0f);
	l_BaseColor.xyz=lerp(l_BaseColor.xyz, 0.0f.xxx, l_BaseColor.w);
	l_BaseColor.xyz=pow(abs(l_BaseColor.xyz), 2.2);
	
	float l_Depth=T3Texture.Sample(S3Sampler, IN.UV).r;
	float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView,	m_InverseProjection);
	
	float4 l_Normal=T2Texture.Sample(S2Sampler, IN.UV);
	float3 l_WorldNormal=Decode(l_Normal.zw);
	
	float3 l_DiffuseContrib;
	float3 l_SpecularContrib;
	CalculatePBRSingleLight(0, l_BaseColor, l_WorldPosition, l_WorldNormal, l_SpecularColor, l_Roughness, l_DiffuseContrib, l_SpecularContrib);
	l_OUT.Target0=float4(l_DiffuseContrib,1);
	l_OUT.Target1=float4(l_SpecularContrib,1);
	
	return l_OUT;
}
	
