#define MAX_OBJECT_RAW_PARAMETER 16
#define MAX_LIGHTS_BY_SHADER 4

cbuffer PerMaterial : register( b0 )
{
	float4 m_RawData[MAX_OBJECT_RAW_PARAMETER];
}

cbuffer PerLight : register( b1 )
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
static float m_RawDataValues[64]=((float[64])m_RawData);