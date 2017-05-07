#include "Samplers.fxh"
#include "globals.fx"

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float2 UV :
    TEXCOORD0;
};


static float m_BloomActive= m_RawData[5].x;
static float m_BloomIntensity= m_RawData[6].x;
static float m_OriginalIntensity= m_RawData[7].x;
static float m_BloomSaturation=m_RawData[8].x;
static float m_OriginalSaturation=m_RawData[9].x;


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

static float m_Threshold=m_RawData[4].x;

float4 BloomPS(PS_INPUT IN) : SV_Target
{
	float l_Threshold=m_RawData[4].x;
	float4 l_Color=T0Texture.Sample(S0Sampler, IN.UV);
	return saturate((l_Color-l_Threshold)/(1-l_Threshold));
}