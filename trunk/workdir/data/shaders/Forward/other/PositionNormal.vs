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

struct App2Vertex
{
	float3 pos    : POSITION;
	float3 normal : NORMAL;
};

struct Vertex2Pixel
{
	float4 pos         : SV_POSITION;
	float3 worldNormal : TEXCOORD1;
};

Vertex2Pixel mainVertexShader( App2Vertex IN )
{
	Vertex2Pixel l_Output = (Vertex2Pixel)0;
	float4 lPos = float4( IN.pos.xyz, 1.0 );
	l_Output.pos = mul( lPos, m_World );
	//l_Output.pos = mul( l_Output.pos, m_View );
	//l_Output.pos = mul( l_Output.pos, m_Projection );
	l_Output.pos = mul( l_Output.pos, m_ViewProjection );
	l_Output.worldNormal = normalize(mul(normalize(IN.normal).xyz, (float3x3)m_World));
	return l_Output;
}