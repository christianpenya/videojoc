#include "Colors.fxh"
#include "globals.fx"

Texture2D DiffuseTexture :
register(t0);
SamplerState DiffuseSampler :
register(s0);

Texture2D LightmapTexture :
register(t1);
SamplerState LightmapSampler :
register(s1);

float g_EnvironmentFactor = 25.0;

struct VS_INPUT
{
float3 Pos :
    POSITION;
float3 Normal :
    NORMAL;
float2 UV :
    TEXCOORD0;
float2 UV2 :
    TEXCOORD1;
};

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float3 Normal :
    NORMAL;
float2 UV :
    TEXCOORD0;
float2 UV2 :
    TEXCOORD1;
float3 WorldNormal :
    TEXCOORD2;
float3 WorldPosition :
    TEXCOORD3;
};

PS_INPUT VS( VS_INPUT IN )
{
    PS_INPUT l_Output = (PS_INPUT)0;
    /*float4 lPos = float4( IN.Pos.xyz, 1.0 );
    l_Output.Pos = mul( lPos, m_World );
    l_Output.WorldPosition=l_Output.Pos.xyz;
    l_Output.Pos = mul( l_Output.Pos, m_View );
    l_Output.Pos = mul( l_Output.Pos, m_Projection );
    l_Output.Normal=normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
    l_Output.WorldNormal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
    l_Output.UV = IN.UV;
    l_Output.UV2 = IN.UV2;*/

	float3 l_Normal=normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	float3 l_EyeToWorldPosition=normalize(IN.WorldPosition-m_InverseView[3].xyz);
	float3 l_ReflectVector=normalize(reflect(l_EyeToWorldPosition, l_Normal));
	float3 l_ReflectColor=LightmapTexture.Sample(LightmapSampler, l_ReflectVector).xyz*g_EnvironmentFactor;

	
    return l_ReflectColor;
}

float4 PS( PS_INPUT IN ) : SV_Target
{
    float3 NormalPixel = normalize(IN.Normal);

    float3 WorldPos = IN.WorldPosition;
    float4 ColorPixel = DiffuseTexture.Sample(DiffuseSampler, IN.UV) * float4(m_RawData[0].xyz, 1.0);
    float4 l_LightmapPixel = LightmapTexture.Sample(LightmapSampler, IN.UV2) * 2;
    //Doblamos contribucion del lightmap para equiparar efecto de 3DSMax

    float3 l_LAmbient = m_LightAmbient.xyz * l_LightmapPixel.xyz * ColorPixel;
//	l_LightmapPixel.xyz * ColorPixel.xyz;
    float3 DiffuseColor = float3(0.0, 0.0, 0.0);
    float3 SpecularColor = float3(0.0, 0.0, 0.0);

    float3 l_LDiffuseSpecular = float3(0.0, 0.0, 0.0);
    float3 l_LDiffuseSpecularTmp = float3(0.0, 0.0, 0.0);

    for (int IdLight = 0; IdLight<MAX_LIGHTS_BY_SHADER; ++IdLight)
    {
        CalculateSingleLight(IdLight, NormalPixel, WorldPos, ColorPixel, DiffuseColor, SpecularColor);

        l_LDiffuseSpecularTmp = DiffuseColor + SpecularColor;
        l_LDiffuseSpecular = l_LDiffuseSpecular + l_LDiffuseSpecularTmp;

        DiffuseColor = float3(0.0, 0.0, 0.0);
        SpecularColor = float3(0.0, 0.0, 0.0);
    }

    return float4(l_LAmbient + l_LDiffuseSpecular, 1.0);
}
