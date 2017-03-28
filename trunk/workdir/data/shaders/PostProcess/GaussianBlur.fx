static float m_TextureSize=m_RawDataValues[0];
static float m_BlurScale=m_RawDataValues[1];
float4 PSBlur(PS_INPUT IN) : SV_Target
{
return GetGaussianBlurFromSampler(T0Texture, S0Sampler, IN.UV, 1/m_TextureSize,
m_BlurScale);
}