//Globals.fxh
#define MAXBONES 40
#define MAX_LIGHTS_BY_SHADER 4
#define MAX_OBJECT_RAW_PARAMETER 16
/*
cbuffer SceneConstantBuffer : register( b0 )
{
	float4x4 m_World;
	float4x4 m_View;
	float4x4 m_Projection;
	float4 m_CameraPosition;
	float4 m_CameraRightVector;
	float4 m_CameraUpVector;
}
cbuffer LightsConstantBuffer : register (b1)
{
	float4 m_LightAmbient;
	float4 m_LightEnabled;
	float4 m_LightType; //0 : OMNI, 1 : DIRECTIONAL, 2 : SPOT
	float4 m_LightPosition[MAX_LIGHTS_BY_SHADER];
	float4 m_LightDirection[MAX_LIGHTS_BY_SHADER];
	float4 m_LightAngle;
	float4 m_LightFallOffAngle;
	float4 m_LightAttenuationStartRange;
	float4 m_LightAttenuationEndRange;
	float4 m_LightIntensity;
	float4 m_LightColor[MAX_LIGHTS_BY_SHADER];
}
cbuffer AnimatedModelConstantBuffer : register (b2)
{
	float4x4 m_Bones[MAXBONES];
}

static float m_LightEnabledArray[4]=(float[4])m_LightEnabled;
static float m_LightTypeArray[4]=(float[4])m_LightType;
static float m_LightAngleArray[4]=(float[4])m_LightAngle;
static float m_LightFallOffAngleArray[4]=(float[4])m_LightFallOffAngle;
static float m_LightAttenuationStartRangeArray[4]=(float[4])m_LightAttenuationStartRange;
static float m_LightAttenuationEndRangeArray[4]=(float[4])m_LightAttenuationEndRange;
static float m_LightIntensityArray[4]=(float[4])m_LightIntensity;

*/
//VERTEX SHADER

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
/*
cbuffer PerAnimatedInstance : register( b2 )
{
	float4x4 m_Bones[MAX_BONES];
}
*/
cbuffer PerLightsVS : register( b3 )
{
	float4 mVS_LightAmbient;	
	float4 mVS_LightEnabled;	
	float4 mVS_LightType;
	float4 mVS_LightPosition[MAX_LIGHTS_BY_SHADER];
	float4 mVS_LightDirection[MAX_LIGHTS_BY_SHADER];
	float4 mVS_LightAngle;
	float4 mVS_LightFallOffAngle;
	float4 mVS_LightAttenuationStartRange;
	float4 mVS_LightAttenuationEndRange;
	float4 mVS_LightIntensity;
	float4 mVS_LightColor[MAX_LIGHTS_BY_SHADER];
}


/*


//PIXEL SHADER

cbuffer PerMaterial : register( b0 )
{
	float4 m_RawData[MAX_OBJECT_RAW_PARAMETER];
}
*/
cbuffer PerLightPS : register( b1 )
{
	float4 mPS_LightAmbient;
	float4 mPS_LightEnabled;
	float4 mPS_LightType;
	float4 mPS_LightPosition[MAX_LIGHTS_BY_SHADER];
	float4 mPS_LightDirection[MAX_LIGHTS_BY_SHADER];
	float4 mPS_LightAngle;
	float4 mPS_LightFallOffAngle;
	float4 mPS_LightAttenuationStartRange;
	float4 mPS_LightAttenuationEndRange;
	float4 mPS_LightIntensity;
	float4 mPS_LightColor[MAX_LIGHTS_BY_SHADER];
}

//static float m_RawDataValues[64]=((float[64])m_RawData);
