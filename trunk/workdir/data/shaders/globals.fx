//Globals.fxh
#define MAXBONES 40
#define MAX_LIGHTS_BY_SHADER 4
#define MAX_OBJECT_RAW_PARAMETER 16

cbuffer PerFrame : register( b0 )
{
    float4x4 m_View;
    float4x4 m_Projection;
    float4x4 m_ViewProjection;
    float4 m_CameraPosition;
    float4 m_CameraFrontVector;
    float4 m_CameraUpVector;
    float4x4 m_InverseView;
    float4x4 m_InverseProjection;
	float4 m_CameraInfo;//x = near, y = far, z = fov, w = aspect ratio
	float4 m_Time;
	float4 m_CameraPositionInScreen;

}

cbuffer PerObject : register( b1 )
{
    float4x4 m_World;
}

cbuffer PerMaterial : register( b2 )
{
    float4 m_RawData[MAX_OBJECT_RAW_PARAMETER];
	float2 m_ViewPosition[MAX_OBJECT_RAW_PARAMETER];
	float2 m_ViewSize[MAX_OBJECT_RAW_PARAMETER];
}

cbuffer AnimatedModelConstantBuffer : register (b3)
{
    float4x4 m_Bones[MAXBONES];
}

cbuffer LightsConstantBuffer : register (b4)
{
    float4 m_LightAmbient;
    float4 m_FogColor;
    float4 m_FogInfo;
    float4 m_LightEnabled;
    float4 m_LightType; //0 : OMNI, 1 : SPOT , 2 : DIRECTIONAL
    float4 m_LightPosition[MAX_LIGHTS_BY_SHADER];
    float4 m_LightDirection[MAX_LIGHTS_BY_SHADER];
    float4 m_LightAngle;
    float4 m_LightFallOffAngle;
    float4 m_LightAttenuationStartRange;
    float4 m_LightAttenuationEndRange;
    float4 m_LightIntensity;
    float4 m_LightColor[MAX_LIGHTS_BY_SHADER];
    float4 m_UseShadowMap;
    float4 m_UseShadowMask;
    float4 m_ShadowMapBias;
    float4 m_ShadowMapStrength;
    float4x4 m_LightView[MAX_LIGHTS_BY_SHADER];
    float4x4 m_LightProjection[MAX_LIGHTS_BY_SHADER];
}

static float m_LightEnabledArray[4]=(float[4])m_LightEnabled;
static float m_LightTypeArray[4]=(float[4])m_LightType;
static float m_LightAngleArray[4]=(float[4])m_LightAngle;
static float m_LightFallOffAngleArray[4]=(float[4])m_LightFallOffAngle;
static float m_LightAttenuationStartRangeArray[4]=(float[4])m_LightAttenuationStartRange;
static float m_LightAttenuationEndRangeArray[4]=(float[4])m_LightAttenuationEndRange;
static float m_LightIntensityArray[4]=(float[4])m_LightIntensity;

static float m_FogColorArray[4]=(float[4])m_FogColor;
static float m_FogInfoArray[4]=(float[4])m_FogInfo;
static float m_UseShadowMapArray[4]=(float[4])m_UseShadowMap;
static float m_UseShadowMaskArray[4]=(float[4])m_UseShadowMask;
static float m_ShadowMapBiasArray[4]=(float[4])m_ShadowMapBias;
static float m_ShadowMapStrengthArray[4]=(float[4])m_ShadowMapStrength;

static float3 m_Color = m_RawData[0].xyz;
static float m_Metalness = m_RawData[1].x;
static float m_Roughness = m_RawData[2].x;
static float m_Occlusion = m_RawData[3].x;

static float m_RawDataValues[64]=((float[64])m_RawData);

void CalculateSingleLight(uint IdLight,float3 NormalPixel : NORMAL, float3 WorldPos : TEXCOORD2, float3 ColorPixel : COLOR, out float3 DiffuseColor : COLOR, out float3 SpecularColor : COLOR)
{

    if(m_LightEnabledArray[IdLight]==1)
    {

        float g_SpecularExponent = 0.5;
		float g_SpecularContrib = 0.5;

        float4 l_LightDirection = m_LightDirection[IdLight];
        float l_DistanceAtten = 1.0;
        float l_AngleAtenuation = 1.0;

		if(m_LightTypeArray[IdLight] == 0)
        {
            // OMNI
            float l_DistanceToPixel = length(float4(WorldPos, 0.0) - m_LightPosition[IdLight]);
            l_LightDirection = (float4(WorldPos, 0.0) - m_LightPosition[IdLight])/l_DistanceToPixel;
            l_DistanceAtten = 1.0 - saturate((l_DistanceToPixel - m_LightAttenuationStartRangeArray[IdLight])/(m_LightAttenuationEndRangeArray[IdLight]-m_LightAttenuationStartRangeArray[IdLight]));
        }
        else if(m_LightTypeArray[IdLight]==1)
        {
            //SPOT
            float l_DistanceToPixel = length(float4(WorldPos, 0.0) - m_LightPosition[IdLight]);
            l_LightDirection = (float4(WorldPos, 0.0) - m_LightPosition[IdLight])/l_DistanceToPixel;
            l_DistanceAtten = 1.0 - saturate((l_DistanceToPixel - m_LightAttenuationStartRangeArray[IdLight])/(m_LightAttenuationEndRangeArray[IdLight]-m_LightAttenuationStartRangeArray[IdLight]));

            float l_SpotAngle=cos(m_LightAngleArray[IdLight]*0.5*(3.1416/180.0));
            float l_SpotFallOff=cos(m_LightFallOffAngleArray[IdLight]*0.5*(3.1416/180.0));
            float l_DotAngle=dot(l_LightDirection, m_LightDirection[IdLight]);

            float l_AngleAtenuation = saturate((l_DotAngle-l_SpotFallOff)/(l_SpotAngle-l_SpotFallOff));
        }
		
		// No hacen falta calculos adicionales para la luz direccional
        
        float l_DiffuseContrib = dot(NormalPixel,(-l_LightDirection.xyz));
		l_DiffuseContrib = max(0, l_DiffuseContrib);
        
		float3 l_Eye = m_CameraPosition.xyz;
        float3 l_ViewDir = normalize(l_Eye - WorldPos);
        float3 Hn = normalize(l_ViewDir-l_LightDirection.xyz);
		float l_SpecularContrib = pow(saturate(dot(Hn,NormalPixel)), 20.0 );

		DiffuseColor = l_DiffuseContrib * m_LightIntensity[IdLight] * m_LightColor[IdLight].xyz * ColorPixel.xyz * l_DistanceAtten * l_AngleAtenuation;
		SpecularColor = l_SpecularContrib * m_LightIntensity[IdLight] *	m_LightColor[IdLight].xyz *g_SpecularContrib * l_DistanceAtten * l_AngleAtenuation;
	}
}

float3 Normal2Texture(float3 Normal)
{
    return (Normal/2.0)+0.5;
}

float3 Texture2Normal(float3 Color)
{
    return (Color-0.5)*2;
}

float3 GetPositionFromZDepthViewInViewCoordinates(float ZDepthView, float2 UV, float4x4 InverseProjection)
{
	// Get the depth value for this pixel
	// Get x/w and y/w from the viewport position
    float x = UV.x * 2 - 1;
    float y = (1 - UV.y) * 2 - 1;
    float4 l_ProjectedPos = float4(x, y, ZDepthView, 1.0);
	// Transform by the inverse projection matrix
    float4 l_PositionVS = mul(l_ProjectedPos, InverseProjection);
	// Divide by w to get the view-space position
    return l_PositionVS.xyz / l_PositionVS.w;
}

float3 GetPositionFromZDepthView(float ZDepthView, float2 UV, float4x4 InverseView, float4x4 InverseProjection)
{
    float3 l_PositionView=GetPositionFromZDepthViewInViewCoordinates(ZDepthView, UV, InverseProjection);
    return mul(float4(l_PositionView,1.0), InverseView).xyz;
}
// Check if enabled