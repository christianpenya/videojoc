#include "globals.fx"

Texture2D DiffuseTexture :
register(t0);
SamplerState LinearSampler :
register(s0);

Texture2D NormalMapTexture :
register(t1);
SamplerState NormalMapTextureSampler :
register(s1);

struct VS_INPUT
{
float3 Pos :
    POSITION;
float3 Normal :
    NORMAL;
float2 UV :
    TEXCOORD0;
float3 Tangent:
    TANGENT;
float3 Binormal:
    BINORMAL;
};

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float3 Normal :
    NORMAL;
float3 Tangent :
    TANGENT;
float3 Binormal :
    BINORMAL;
float2 UV :
    TEXCOORD0;
float3 WorldPosition :
    TEXCOORD1;
};

PS_INPUT VS(VS_INPUT IN)
{
    PS_INPUT l_Output = (PS_INPUT)0;
    float4 lPos = float4(IN.Pos.xyz, 1.0);
    l_Output.Pos = mul(lPos, m_World);
    l_Output.WorldPosition = l_Output.Pos.xyz;
    l_Output.Pos = mul(l_Output.Pos, m_View);
    l_Output.Pos = mul(l_Output.Pos, m_Projection);

    l_Output.Normal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
    l_Output.Tangent = normalize(mul(IN.Tangent.xyz, (float3x3)m_World));
    l_Output.Binormal = normalize(mul(cross(IN.Tangent.xyz, IN.Normal.xyz), (float3x3)m_World));

    l_Output.UV = IN.UV;

    return l_Output;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	
    float3 bump = m_RawData[1].x * (NormalMapTexture.Sample(NormalMapTextureSampler, IN.UV).rgb - float3(0.5, 0.5, 0.5));
    float3 l_Normal = normalize(IN.Normal);
    l_Normal = l_Normal + bump.x*IN.Tangent + bump.y*IN.Binormal;
    l_Normal = normalize(l_Normal);

    float3 l_WorldPos = IN.WorldPosition;
    float4 l_DiffuseColor = DiffuseTexture.Sample(LinearSampler, IN.UV) * float4(m_RawData[0].xyz, 1.0);

    float3 l_LAmbient = m_LightAmbient.xyz * l_DiffuseColor.xyz;

    float3 l_DiffuseTmp = float3(0.0, 0.0, 0.0);
    float3 l_SpecularTmp = float3(0.0, 0.0, 0.0);

    float3 l_LDiffuseSpecular = float3(0.0, 0.0, 0.0);
    float3 l_LDiffuseSpecularTmp = float3(0.0, 0.0, 0.0);

    for (int i = 0; i<MAX_LIGHTS_BY_SHADER; i++)
    {
        CalculateSingleLight(i, l_Normal, l_WorldPos, l_DiffuseColor, l_DiffuseTmp, l_SpecularTmp);

        l_LDiffuseSpecularTmp = l_DiffuseTmp + l_SpecularTmp;
        l_LDiffuseSpecular = l_LDiffuseSpecular + l_LDiffuseSpecularTmp;

        l_DiffuseTmp = float3(0.0, 0.0, 0.0);
        l_SpecularTmp = float3(0.0, 0.0, 0.0);
    }

    return float4(l_LAmbient + l_LDiffuseSpecular, 1.0);
}