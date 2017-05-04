static float m_Threshold=m_RawDataValues[0];
float4 PSBloom(PS_INPUT IN) : SV_Target
{
float4 l_Color=T0Texture.Sample(S0Sampler, IN.UV);
return saturate((l_Color-m_Threshold)/(1-m_Threshold));
}