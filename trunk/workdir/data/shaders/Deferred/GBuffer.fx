#include "globals.fx"

struct VS_INPUT
{
float3 Pos :
    POSITION;
float3 Normal :
    NORMAL;
};

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float3 WorldNormal :
    TEXCOORD0;
float3 WorldPosition :
    TEXCOORD1;
float2 Depth : 
	TEXCOORD2;
};

struct PixelOutputType
{
float4 Target0 :
    SV_Target0; //Albedo (float3) + (float) SpecularFactor
float4 Target1 :
    SV_Target1; //AmbientLight (float3) + (float) SpecularPow
float4 Target2 :
    SV_Target2; //Normal (float3) + (float) Not used
float4 Target3 :
    SV_Target3; //Depth (float4)
};

PS_INPUT VS( VS_INPUT IN )
{
    PS_INPUT l_Output = (PS_INPUT)0;
    float4 lPos = float4( IN.Pos.xyz, 1.0 );
    l_Output.Pos = mul( lPos, m_World );
    l_Output.WorldPosition = l_Output.Pos.xyz;
    l_Output.Pos = mul( l_Output.Pos, m_View );
    l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.Depth = l_Output.Pos.zw;

	float3 normal = (IN.Normal/2) + 0.5;
	
    l_Output.WorldNormal = normalize(mul(normalize(normal).xyz, (float3x3)m_World));

    return l_Output;
}

PixelOutputType PS(PS_INPUT IN) : SV_Target
{
    PixelOutputType l_Output = (PixelOutputType)0;

    //TODO Pasar parametros especular por rawData
    float g_SpecularExponent = 80.0;
    float g_SpecularContrib = 1.0;
    float 3 l_Normal=Normal2Texture(IN.WorldNormal.xyz);
    l_Output.Target0 = float4(m_RawData[0].xyz, g_SpecularContrib);
    l_Output.Target1 = float4(m_LightAmbient.xyz*m_RawData[0].xyz, g_SpecularExponent);
    l_Output.Target2 = float4(l_Normal, 0.0);
	float l_Depth=IN.Depth.x/IN.Depth.y;
    l_Output.Target3 = float4(l_Depth, l_Depth, l_Depth, 1.0);

    return l_Output;
}
