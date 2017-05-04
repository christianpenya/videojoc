#include "Samplers.fxh"

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float2 UV :
    TEXCOORD0;
};

float4 GetGaussianBlurFromSampler(Texture2D _Texture2D, SamplerState _SamplerState, float2 UV, float2 TextureSize, float TextureScale)
{
    float2 l_PixelKernel[7] =
    {
        { -1, 1 },
        { -1, -1 },
        { -1, 0 },
        { 0, 0 },
        { 1, 0 },
        { 1, -1 },
        { 1, 1 },
    };

    float l_BlurWeights[7] =
    {
        0.120985,
        0.120985,
        0.176033,
        0.199471,
        0.176033,
        0.120985,
        0.120985,
    };

    float4 l_GaussianColor = 0;
    float2 l_TextureSize = TextureSize * TextureScale;
    float l_TotalWeights = 0.0;

    for(int i = 0; i < 7; ++i)
    {
        float4 l_Color = _Texture2D.Sample(_SamplerState, UV+(l_PixelKernel[i] * l_TextureSize))*l_BlurWeights[i];
        l_TotalWeights += l_BlurWeights[i];
        l_GaussianColor += l_Color;
    }

    return l_GaussianColor/l_TotalWeights;
}

/*static float m_TextureSize=m_RawDataValues[0];
static float m_BlurScale=m_RawDataValues[1];
*/

float4 BlurPS(PS_INPUT IN) : SV_Target
{
	float m_TextureSize=512;
	float m_BlurScale=6.0;
	
	//float2 m_TextureSize=512; //m_ViewSize[0];
	//float2 m_BlurScale=3.0
	
	//return GetGaussianBlurFromSampler(T0Texture, S0Sampler, IN.UV, m_TextureSize, m_BlurScale);
    return GetGaussianBlurFromSampler(T0Texture, S0Sampler, IN.UV, float2(1.0/m_TextureSize, 1.0/m_TextureSize), m_BlurScale);
}
