//POSITION_NORMAL_TEXTURE_SHADER.FX
#include "globals.fxh"

Texture2D DiffuseTexture : register( t0 );
SamplerState LinearSampler : register( s0 );

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : POSITION;
	float4 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------


PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos = mul( float4(IN.Pos, 1.0), m_World );
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.UV = IN.UV;
	l_Output.WorldNormal=normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	return l_Output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT IN) : SV_Target
{
	float3 l_Eye=m_CameraPosition;
	float3 l_ViewDir = normalize(l_Eye - IN.WorldPosition);

	float3 Hn[MAX_LIGHTS_BY_SHADER];
	for (i=0;i<MAX_LIGHTS_BY_SHADER;i++){
		Hn[i]= normalize(l_ViewDir-m_LightDirection[i]);
	}

	float3 l_Normal = normalize(IN.Normal);

	float l_DiffuseContrib[MAX_LIGHTS_BY_SHADER];
	for (i=0;i<MAX_LIGHTS_BY_SHADER;i++){	
		l_DiffuseContrib[i] = saturate(dot(-m_LightDirection[i],l_Normal));
	}

	//float l_SpecularContrib = pow(saturate(dot(Hn,l_Normal)), g_SpecularExponent ) ;
	float4 l_DiffuseColor =	DiffuseTexture.Sample(DiffuseSampler, IN.UV);
	float3 l_LAmbient = m_LightAmbient * l_DiffuseColor.xyz;

	float3 l_LDiffuse;
	for (i=0;i<MAX_LIGHTS_BY_SHADER;i++){
		l_LDiffuse+=l_DiffuseContrib[i] * m_LightIntensity * m_LightColor[i] * l_DiffuseColor.xyz;
	}
	//float3 l_LDiffuse = l_DiffuseContrib * m_LightIntensity * Lamp_0_color * l_DiffuseColor.xyz;
	//float3 l_LSpecular = l_SpecularContrib* m_LightIntensity *	Lamp_0_color *g_SpecularContrib;

	float3 l_SumLDiffuse;
	for (i=0;i<MAX_LIGHTS_BY_SHADER;i++){
		l_LDiffuse[i]
	}

	return float4( l_LAmbient+l_Diffuse,1.0);
	//return float4( l_LAmbient+l_LDiffuse+l_LSpecular,1.0);
	
}