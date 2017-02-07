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

	float3 l_Eye=m_CameraPosition.xyz;

	float3 l_ViewDir = normalize(l_Eye - IN.WorldPosition);
	
	//float3 Hn = normalize(l_ViewDir-m_LightDirection[0].xyz);
	float3 l_Normal = normalize(IN.Normal);

	
	float l_DiffuseContrib = saturate(dot(-m_LightDirection[0],l_Normal));
	float l_SpecularContrib = pow(saturate(dot(Hn,l_Normal)), 20.0 ) ;
	float4 l_DiffuseColor =	float4(1.0 , 1.0 , 1.0, 1.0);


	float3 l_LAmbient = m_LightAmbient.xyz * l_DiffuseColor.xyz;

	float3 l_LDiffuse = l_DiffuseContrib * m_LightIntensity[0] * m_LightColor[0].xyz * l_DiffuseColor.xyz;


	float3 l_LSpecular = l_SpecularContrib* m_LightIntensity[0] *	m_LightColor[0].xyz *g_SpecularContrib;

	//return float4(l_SpecularContrib, l_SpecularContrib, l_SpecularContrib, 1.0);
	//return float4(1.0 ,0.0 ,0.0 ,1.0);
	//return float4(l_LDiffuse,1.0);
	//return float4(m_LightColor[0].xyz,1.0);
	//return float4( l_LAmbient+l_LDiffuse,1.0);

	return float4(l_LAmbient+l_LDiffuse+l_LSpecular,1.0);
	//return float4(l_DiffuseContrib,l_DiffuseContrib,l_DiffuseContrib,1.0);
	return float4(l_Normal,1.0);
	return float4(normalize(m_LightDirection[0].xyz),1.0);

}

/*
	float3 l_Eye=ViewInverse[3].xyz;
	float3 l_ViewDir = normalize(l_Eye - IN.WorldPosition);
	float3 Hn = normalize(l_ViewDir-Lamp0Direction);
	float3 l_Normal = normalize(IN.Normal);
	float l_DiffuseContrib = saturate(dot(-Lamp0Direction,l_Normal));
	float l_SpecularContrib = pow(saturate(dot(Hn,l_Normal)), g_SpecularExponent ) ;
	float4 l_DiffuseColor =	tex2D(DiffuseSampler, IN.UV);
	float3 l_LAmbient = AmbientLight * l_DiffuseColor.xyz;
	float3 l_LDiffuse = l_DiffuseContrib * g_LigthIntensity * Lamp_0_color * l_DiffuseColor.xyz;
	float3 l_LSpecular = l_SpecularContrib* g_LigthIntensity *	Lamp_0_color *g_SpecularContrib;

	return float4( l_LAmbient+l_LDiffuse+l_LSpecular,1.0);
*/

