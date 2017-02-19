#include "globals.fx"
Texture2D T0Texture: register( t0 ); // Diffuse
SamplerState S0Sampler: register( s0 );

struct Vertex2Pixel
{
	float4 pos : SV_POSITION;
	float2 UV : TEXCOORD0;
	float3 worldNormal : TEXCOORD1;

};

struct App2Vertex
{
	float3 pos : POSITION;
	float4 Weight :BLENDWEIGHT;
	float4 Indices :BLENDINDICES;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};
 

Vertex2Pixel mainVertexShader( App2Vertex IN )
{
	Vertex2Pixel l_Output = (Vertex2Pixel)0;
	
	float4 l_TempPos=float4(IN.pos.xyz, 1.0);
	float4 l_Indices=IN.Indices;
	float4 l_Position;
	float3 l_Normal=float3(0,0,0);
	
	l_Position=mul(l_TempPos, m_Bones[l_Indices.x]) * IN.Weight.x;
	l_Position+=mul(l_TempPos, m_Bones[l_Indices.y]) * IN.Weight.y;
	l_Position+=mul(l_TempPos, m_Bones[l_Indices.z]) * IN.Weight.z;
	l_Position+=mul(l_TempPos, m_Bones[l_Indices.w]) * IN.Weight.w;
	float3x3 m;
	m[0].xyz = m_Bones[l_Indices.x][0].xyz;
	m[1].xyz = m_Bones[l_Indices.x][1].xyz;
	m[2].xyz = m_Bones[l_Indices.x][2].xyz;
	l_Normal+=mul(IN.Normal.xyz, m)* IN.Weight.x;
	m[0].xyz = m_Bones[l_Indices.y][0].xyz;
	m[1].xyz = m_Bones[l_Indices.y][1].xyz;
	m[2].xyz = m_Bones[l_Indices.y][2].xyz;
	l_Normal+=mul(IN.Normal.xyz, m)* IN.Weight.y;
	l_Normal=normalize(l_Normal);
	l_Output.pos = mul( l_Position, m_World );
	
	l_Output.pos = mul( l_Output.pos, m_ViewProjection );
	l_Output.worldNormal = normalize(mul(l_Normal, (float3x3)m_World));
	l_Output.UV=IN.UV;
	
	return l_Output;
}

float4 mainPixelShader( Vertex2Pixel pix ) : SV_Target
{
	return T0Texture.Sample(S0Sampler, pix.UV);
}