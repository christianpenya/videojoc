#define MAX_BONES 40
#define MAX_LIGHTS_BY_SHADER 4

cbuffer PerFrame : register( b0 )
{
	float4x4 m_View;
	float4x4 m_Projection;
	float4x4 m_ViewProjection;
}

cbuffer PerObject : register( b1 )
{
	float4x4 m_World;
}

cbuffer PerAnimatedInstance : register( b2 )
{
	float4x4 m_Bones[MAX_BONES];
}

cbuffer PerLights : register( b3 )
{
	float4 m_LightAmbient;
	float4 m_LightEnabled;
	float4 m_LightType;
	float4 m_LightPosition[MAX_LIGHTS_BY_SHADER];
	float4 m_LightDirection[MAX_LIGHTS_BY_SHADER];
	float4 m_LightAngle;
	float4 m_LightFallOffAngle;
	float4 m_LightAttenuationStartRange;
	float4 m_LightAttenuationEndRange;
	float4 m_LightIntensity;
	float4 m_LightColor[MAX_LIGHTS_BY_SHADER];
}