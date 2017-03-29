#include "globals.fx"
#include "Samplers.fxh"

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
float3 WorldPos :
    TEXCOORD1;
};

PS_INPUT VS(VS_INPUT IN)
{
    PS_INPUT l_Output = (PS_INPUT)0;
    l_Output.Pos = float4(IN.Pos, 1.0);
    l_Output.WorldPos = mul(l_Output.Pos, m_World);
    l_Output.UV = IN.UV;

    return l_Output;
}

float4 PS(PS_INPUT IN) : SV_Target
{
    float4 lHola = T0Texture.Sample(S0Sampler, IN.UV);
    return lHola;
    /*
    float l_DistanceEyeToWorldPosition=length(IN.WorldPosition-m_InverseView[3].xyz);
    return float4(GetFogColor(l_DistanceEyeToWorldPosition, l_FinalColor.xyz), l_FinalColor.a);
    */
}
/*
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
*/