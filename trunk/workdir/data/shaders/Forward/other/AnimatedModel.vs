#define MAX_BONES 		40

cbuffer PerFrame : register( b0 )
{
    float4x4 m_View;
	float4x4 m_Projection;
	float4x4 m_ViewProjection;
}
 
cbuffer PerObject : register( b1 )
{
    float4x4 m_World;
}
cbuffer PerAnimatedInstance : register( b2 )
{
	float4x4 m_Bones[MAX_BONES];
}
struct App2Vertex
{
	float3 pos : POSITION;
	float4 Weight :BLENDWEIGHT;
	float4 Indices :BLENDINDICES;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};
struct Vertex2Pixel
{
	float4 pos : SV_POSITION;
	float2 UV : TEXCOORD0;
	float3 worldNormal : TEXCOORD1;
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