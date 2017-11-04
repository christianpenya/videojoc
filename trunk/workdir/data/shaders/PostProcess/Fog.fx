#include "globals.fx"
#include "Samplers.fxh"

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float2 UV :
    TEXCOORD0;
};

float CalcAttenuation(float Depth, float StartFog, float EndFog)
{
    //StartFog = 10.0;
    //EndFog = 50.0;
    if (Depth < StartFog)
        return 1.0;
    else if (Depth < EndFog)
        return (Depth - EndFog) / (StartFog - EndFog);
    else
        return 0.0;
}

float4 CalcLinearFog(float Depth, float StartFog, float EndFog, float3 FogColor)
{
    return float4(FogColor, 1.0 - CalcAttenuation(Depth, StartFog, EndFog));
    //1.0 - CalcAttenuation(Depth, StartFog, EndFog)
}

float4 CalcExp2Fog(float Depth, float ExpDensityFog, float3 FogColor)
{
    const float LOG2E = 1.442695; // = 1 / log(2)
    float l_Fog = exp2(-ExpDensityFog * ExpDensityFog * Depth * Depth * LOG2E);
    return float4(FogColor, 1.0 - l_Fog);
}

float4 CalcExpFog(float Depth, float ExpDensityFog, float3 FogColor)
{
    const float LOG2E = 1.442695; // = 1 / log(2)
    float l_Fog = exp2(-ExpDensityFog * Depth * LOG2E);
    return float4(FogColor, 1.0 - l_Fog);
}

float3 GetFogColor(float Depth, float3 CurrentColor)
{
    float3 m_FogColor = float4(m_RawData[0].xyz, 1.0);
    float4 m_FogParams = float4(m_RawData[1].xy, 0.0, 0.0);
    float m_StartLinearFog = m_FogParams[0];
    float m_EndLinearFog = m_FogParams[1];
    float4 l_FogColor = CalcLinearFog(Depth, m_StartLinearFog, m_EndLinearFog, m_FogColor);
    //float4 l_FogColor = CalcExp2Fog(Depth, 0.03, m_FogColor);

    return float3(CurrentColor * (1.0 - l_FogColor.a) + l_FogColor.xyz * l_FogColor.a);
}

float4 PS(PS_INPUT IN) : SV_Target
{
	return float4(1,1,0,1);
	return T0Texture.Sample(S0Sampler, IN.UV);
    float4 l_FinalColor = T0Texture.Sample(S0Sampler, IN.UV);
	// return T0Texture.Sample(S0Sampler, IN.UV);
    float l_DepthValue = T1Texture.Sample(S1Sampler, IN.UV).r;

    float3 position = GetPositionFromZDepthView(l_DepthValue, IN.UV, m_InverseView, m_InverseProjection);
    float l_DistanceEyeToWorldPosition = length(position - m_InverseView[3].xyz);

    return float4(GetFogColor(l_DistanceEyeToWorldPosition, l_FinalColor.xyz), l_FinalColor.a);
}
