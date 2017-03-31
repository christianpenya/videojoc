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

PS_INPUT VS(VS_INPUT IN)
{
    PS_INPUT l_Output = (PS_INPUT)0;

    l_Output.Pos = float4(IN.Pos, 1.0);
    l_Output.UV = IN.UV;

    return l_Output;
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

float4 PS(PS_INPUT IN) : SV_Target
{
    float4 l_FinalColor = T0Texture.Sample(S0Sampler, IN.UV);
    float l_DepthValue = T1Texture.Sample(S1Sampler, IN.UV).r;

    float3 position=GetPositionFromZDepthView(l_DepthValue, IN.UV, m_InverseView, m_InverseProjection);
    float l_DistanceEyeToWorldPosition = length(position - m_InverseView[3].xyz);

    return float4(GetFogColor(l_DistanceEyeToWorldPosition, l_FinalColor.xyz), l_FinalColor.a);
}
