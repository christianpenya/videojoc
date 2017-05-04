#include "Samplers.fxh"
#include "globals.fx"

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float2 UV :
    TEXCOORD0;
};


static float m_BloomActive= 1; //m_RawDataValues[0];
static float m_BloomIntensity=1.0; //m_RawDataValues[1];
static float m_OriginalIntensity=0.8; // m_RawDataValues[2];
static float m_BloomSaturation=0.8; // m_RawDataValues[3];
static float m_OriginalSaturation=0.6; // m_RawDataValues[4];


float4 AdjustSaturation(float4 Color, float Saturation)
{
	// We define gray as the same color we used in the grayscale shader
	float l_Grey=dot(Color, float3(0.3, 0.59, 0.11));
	return lerp(l_Grey, Color, Saturation);
}

float4 BloomCombinePS(PS_INPUT IN) : SV_Target
{
	if(m_BloomActive==0.0)
		return T0Texture.Sample(S0Sampler, IN.UV);
	else
	{
		float4 l_BloomColor=T1Texture.Sample(S1Sampler, IN.UV);
		// Get our original pixel from ColorMap
		float4 l_OriginalColor=T0Texture.Sample(S0Sampler, IN.UV);
		// Adjust color saturation and intensity based on the input variables to the shader
		l_BloomColor=AdjustSaturation(l_BloomColor,m_BloomSaturation)*m_BloomIntensity;
		l_OriginalColor=AdjustSaturation(l_OriginalColor,m_OriginalSaturation)*m_OriginalIntensity;
		// make the originalColor darker in very bright areas, avoiding these areas look burned-out
		l_OriginalColor*=(1-saturate(l_BloomColor));
		// Combine the two images.
		return l_OriginalColor+l_BloomColor;
	}
}

//static float m_Threshold=m_RawDataValues[0];

float4 BloomPS(PS_INPUT IN) : SV_Target
{
	float l_Threshold=0.8; //m_RawDataValues[0];
	float4 l_Color=T0Texture.Sample(S0Sampler, IN.UV);
	return saturate((l_Color-l_Threshold)/(1-l_Threshold));
}