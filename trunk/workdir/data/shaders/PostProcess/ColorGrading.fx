#include "globals.fx"
#include "Samplers.fxh"

static float m_Enabled = 1;// m_RawDataValues[0];
static float m_BrownSepiaColorGrading = 0.5; // m_RawDataValues[1];
static float m_GreenSepiaColorGrading = 0.5;// m_RawDataValues[2];
static float m_BlueSepiaColorGrading = 0.5; // m_RawDataValues[3];
static float m_MonochromeColorGrading = 0.5; // m_RawDataValues[4];
static float m_ColorColorGrading = 0.5;// m_RawDataValues[5];
static float m_ContrastColorGrading = 0.5;// m_RawDataValues[6];
static float m_BrightnessColorGrading = 0.5;// m_RawDataValues[7];

struct PS_INPUT
{
	float4 Pos :
	SV_POSITION;
	float2 UV :
	TEXCOORD0;
};

float4 ColorGrading(float4 _Color)
{
	float4x4 gray = { 0.299, 0.587, 0.184, 0,
		0.299, 0.587, 0.184, 0,
		0.299, 0.587, 0.184, 0,
		0, 0, 0, 1 };
	float4x4 sepia = { 0.299, 0.587, 0.184, 0.1,
		0.299, 0.587, 0.184, 0.018,
		0.299, 0.587, 0.184, -0.090,
		0, 0, 0, 1 };
	float4x4 sepia2 = { 0.299, 0.587, 0.184, -0.090,
		0.299, 0.587, 0.184, 0.018,
		0.299, 0.587, 0.184, 0.1,
		0, 0, 0, 1 };
	float4x4 sepia3 = { 0.299, 0.587, 0.184, -0.090,
		0.299, 0.587, 0.184, 0.1,
		0.299, 0.587, 0.184, 0.1,
		0, 0, 0, 1 };
	float3 monochrome = (_Color.r * 0.3f + _Color.g * 0.59f + _Color.b * 0.11f);
		float4 monochrome4 = float4(monochrome, 1);
		float4 result2 = float4(_Color.xyz, 1.0);
		float4 brownsepia = mul(sepia, result2);
		float4 greensepia = mul(sepia3, result2);
		float4 bluesepia = mul(sepia2, result2);
		float4 combine = (brownsepia *m_BrownSepiaColorGrading) +
		(greensepia*m_GreenSepiaColorGrading) + (bluesepia * m_BlueSepiaColorGrading) + (monochrome4
		* m_MonochromeColorGrading) + (m_ColorColorGrading * result2);
	return (combine * m_ContrastColorGrading) + m_BrightnessColorGrading;
}

float4 PSGUIColorGrading(PS_INPUT IN) : SV_Target
{
	//return float4(1.0, 0.0, 0.0, 1.0);
	float4 l_Color = T0Texture.Sample(S0Sampler, IN.UV);
	if (!m_Enabled)
		return l_Color;
	return ColorGrading(l_Color);
}