#include "globals.fx"
#include "Samplers.fxh"

#if USE_UV
	Texture2D DiffuseTexture :
	register(t0);
	SamplerState LinearSampler :
	register(s0);

	#if USE_UV2
		Texture2D LightmapTexture :
		register(t1);
		SamplerState LightmapSampler :
		register(s1);
	#endif
	#if USE_BUMP
		Texture2D NormalMapTexture :
		register(t1);
		SamplerState NormalMapTextureSampler :
		register(s1);
	#endif
#endif

struct VS_INPUT
{
float3 Pos :
    POSITION;
float3 Normal :
    NORMAL;
#if USE_WEIGHTIDX
		float4 Weight :
			BLENDWEIGHT;
		float4 Indices :
			BLENDINDICES;
#endif
#if USE_UV
	float2 UV :
	    TEXCOORD0;
	#if USE_UV2
		float2 UV2 :
		    TEXCOORD1;
	#endif
	#if USE_BUMP
		float3 Tangent:
		    TANGENT;
		float3 Binormal:
		    BINORMAL;
	#endif
#endif
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Depth : TEXCOORD0;
	float2 UV : TEXCOORD1;
};

PS_INPUT VS( VS_INPUT IN )
{   
	PS_INPUT l_Out=(PS_INPUT)0;
	
	#if USE_WEIGHTIDX
		float4 l_TempPos=float4(IN.Pos.xyz, 1.0);
		float4 l_Indices=IN.Indices;
		float3 lPos;
		lPos=mul(l_TempPos, m_Bones[l_Indices.x]) * IN.Weight.x;
		lPos+=mul(l_TempPos, m_Bones[l_Indices.y]) * IN.Weight.y;
		lPos+=mul(l_TempPos, m_Bones[l_Indices.z]) * IN.Weight.z;
		lPos+=mul(l_TempPos, m_Bones[l_Indices.w]) * IN.Weight.w;
		
		float3 l_Normal=float3(0,0,0);	
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
		
		l_Out.Pos = mul(float4(lPos.xyz, 1.0), m_World);
	#else
		l_Out.Pos=mul(float4(IN.Pos.xyz, 1.0), m_World);
	#endif
	
	l_Out.Pos=mul(l_Out.Pos, m_View);
	l_Out.Pos=mul(l_Out.Pos, m_Projection);
	l_Out.Depth=l_Out.Pos;
	
	#if USE_UV
		l_Out.UV = IN.UV;
	#endif
	
	return l_Out;
}

float4 PS(PS_INPUT IN) : SV_Target
{	
	#if USE_UV
		if (DiffuseTexture.Sample(LinearSampler, IN.UV).w <0.1)
		{
			clip(-1);
		}
	#endif
	
	float l_Depth=IN.Depth.z/IN.Depth.w;
	return float4(l_Depth, l_Depth, l_Depth, 1);
}
