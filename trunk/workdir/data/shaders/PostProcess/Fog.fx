#include "globals.fx"

Texture2D T0Texture :
register(t0);
SamplerState S0Sampler :
register(s0);

Texture2D T1Texture :
register(t1);
SamplerState S1Sampler :
register(s1);

struct VS_INPUT
{
float3 Pos :
    POSITION;
float2 UV :
    TEXCOORD0;
};

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float2 UV :
    TEXCOORD0;
};

PS_INPUT VS(VS_INPUT IN)
{
    PS_INPUT l_Output = (PS_INPUT)0;
    l_Output.Pos = float4(IN.Pos, 1.0);
    l_Output.UV = IN.UV;
    return l_Output;
}

float4 PS(PS_INPUT IN) : SV_Target
{
    return float4(1.0, 0.0, 0.0, 1.0);
    float l_NoiseX = m_Time.x*sin(IN.UV.x*IN.UV.y+m_Time.x);
    l_NoiseX = fmod(l_NoiseX, 8)*fmod(l_NoiseX, 4);
    float l_DistortX = fmod(l_NoiseX, m_NoiseAmount);
    float l_DistortY = fmod(l_NoiseX, m_NoiseAmount+0.002);
    float2 l_DistortUV = float2(l_DistortX, l_DistortY);
    float4 l_Noise = T0Texture.Sample(S0Sampler, IN.UV+l_DistortUV)*m_NoisePct;

    float4 l_Vignetting=T1Texture.Sample(S1Sampler, IN.UV)*m_VignettingPct;
    return l_Noise + l_Vignetting;
}

float4 CalcLinearFog(float Depth, float StartFog, float EndFog, float3 FogColor)
{
	return float4(FogColor, 1.0-CalcAttenuation(Depth, StartFog, EndFog));
}

float4 CalcExp2Fog(float Depth, float ExpDensityFog, float3 FogColor)
{
const float LOG2E = 1.442695; // = 1 / log(2)
float l_Fog = exp2(-ExpDensityFog * ExpDensityFog * Depth * Depth * LOG2E);
return float4(FogColor, 1.0-l_Fog);
}
float4 CalcExpFog(float Depth, float ExpDensityFog, float3 FogColor)
{
const float LOG2E = 1.442695; // = 1 / log(2)
float l_Fog = exp2(-ExpDensityFog * Depth * LOG2E);
return float4(FogColor, 1.0-l_Fog);
}
float3 GetFogColor(float Depth, float3 CurrentColor)
{
float4 l_FogColor=CalcLinearFog(Depth, m_StartLinearFog, m_EndLinearFog, m_FogColor);
return float3(CurrentColor*(1.0-l_FogColor.a)+l_FogColor.xyz*l_FogColor.a);
}
