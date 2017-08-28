#include "globals.fx"
#include "Samplers.fxh"

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float2 UV :
    TEXCOORD0;
};

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

float4 PS(PS_INPUT IN) : SV_Target
{
	float l_Depth=T3Texture.Sample(S3Sampler, IN.UV).r;
	float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView,m_InverseProjection);
	float4 l_BaseColor=T0Texture.Sample(S0Sampler, IN.UV);
	float4 l_SpecularColor=float4(lerp(	0.04f.xxx, l_BaseColor.xyz, l_BaseColor.w), 1.0f);
	float4 l_AmbientColor=T1Texture.Sample(S1Sampler, IN.UV);
		
	float l_Occlusion=1-(1-l_AmbientColor.a)*0.75;
	l_AmbientColor.xyz=pow(abs(l_AmbientColor.xyz), 2.2);
	l_AmbientColor.xyz*=l_Occlusion;
	
	float4 l_SpecularEnvironmentColor=T4Texture.Sample(S1Sampler, IN.UV);

	l_SpecularEnvironmentColor=pow(abs(l_SpecularEnvironmentColor), 2.2);
	l_SpecularEnvironmentColor*=l_Occlusion;
	l_BaseColor.xyz=lerp(l_BaseColor.xyz, 0.0f.xxx, l_BaseColor.w);
	l_BaseColor.xyz=pow(abs(l_BaseColor.xyz), 2.2);
	
	float4 l_Normal=T2Texture.Sample(S2Sampler, IN.UV);
	float3 l_NormalPlane=Decode(l_Normal.xy);
	float3 l_NormalPixel=Decode(l_Normal.zw);		

	float3 l_CameraToPixel=normalize(l_WorldPosition-m_InverseView[3].xyz);
	float3 l_NormalDotCam=max(dot(lerp(l_NormalPlane, l_NormalPixel,max(dot(l_NormalPlane, -l_CameraToPixel), 0)), -l_CameraToPixel), 0);
	float3 l_ShlickFresnel=saturate(l_SpecularColor.xyz+(1-l_SpecularColor.xyz)*pow(1-l_NormalDotCam.xyz, 5));
	
	float3 l_DiffuseLight=T5Texture.Sample(S5Sampler, IN.UV).xyz;
	float3 l_SpecularLight=T6Texture.Sample(S6Sampler, IN.UV).xyz;
	
	l_DiffuseLight.xyz+=l_AmbientColor.xyz*l_BaseColor.xyz;
	float3 l_FinalColor=0;
	l_FinalColor.xyz=lerp(l_DiffuseLight.xyz, l_SpecularEnvironmentColor.xyz,l_ShlickFresnel);
	l_FinalColor.xyz+=l_SpecularLight.xyz;
	l_FinalColor.xyz=pow(abs(l_FinalColor.xyz), .4545);

	return float4(l_FinalColor.xyz, 1.0);
}
