#include "Colors.fxh"
#include "globals.fxh"



struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	//float2 UV : TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : SV_POSITION; // COMENTAR JORDI
	float3 Normal : NORMAL;
	//float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float3 WorldPosition : TEXCOORD2;
};


PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	float4 lPos = float4( IN.Pos.xyz, 1.0 );
	l_Output.Pos = mul( lPos, m_World );
	l_Output.WorldPosition=l_Output.Pos.xyz;
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );

	l_Output.Normal=normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	l_Output.WorldNormal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	
	return l_Output;
}

float4 PS( PS_INPUT IN ) : SV_Target{
	float g_SpecularExponent = 80.0;
	float g_SpecularContrib = 1.0;

	//return float4(1.0, 0.0, 0.0, 1.0);
	
	float4 l_DiffuseColor =	float4(1.0 , 1.0 , 1.0, 1.0);
	
	float l_DistanceToPixel = length(IN.WorldPosition - m_LightPosition[0]);
	
	float l_DistanceAtten = 1.0 - saturate((l_DistanceToPixel - m_LightAttenuationStartRangeArray[0])/(m_LightAttenuationEndRangeArray[0]-m_LightAttenuationStartRangeArray[0]));
	
	float3 l_LightDirection = (IN.WorldPosition - m_LightPosition[0])/l_DistanceToPixel;

	float3 l_Eye=m_CameraPosition.xyz;
	float3 l_ViewDir = normalize(l_Eye - IN.WorldPosition);
	
	float3 Hn = normalize(l_ViewDir-l_LightDirection.xyz);
	float3 l_Normal = normalize(IN.Normal);

	
	float l_DiffuseContrib = saturate(dot(-l_LightDirection,l_Normal));
	float l_SpecularContrib = pow(saturate(dot(Hn,l_Normal)), 20.0 ) ;

	
	float3 l_LAmbient = m_LightAmbient.xyz * l_DiffuseColor.xyz;

	float3 l_LDiffuse = l_DiffuseContrib * m_LightIntensity[0] * m_LightColor[0].xyz * l_DiffuseColor.xyz * l_DistanceAtten;

	float3 l_LSpecular = l_SpecularContrib* m_LightIntensity[0] *	m_LightColor[0].xyz *g_SpecularContrib * l_DistanceAtten;



	return float4(l_LAmbient+l_LDiffuse+l_LSpecular,1.0);
	
	
}


