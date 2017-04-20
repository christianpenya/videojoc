#include "globals.fx"


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
#endif

#if USE_BUMP
	Texture2D NormalMapTexture :
	register(t1);
	SamplerState NormalMapTextureSampler :
	register(s1);
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
	#endif
	#if USE_BUMP
		float3 Tangent:
		    TANGENT;
		float3 Binormal:
		    BINORMAL;

	#endif

};


struct PS_INPUT
{
	float4 Pos :
	    SV_POSITION;
	#if USE_BUMP
		float3 Tangent :
		    TANGENT;
		float3 Binormal :
		    BINORMAL;
	#endif
	#if USE_UV
		float2 UV :
		    TEXCOORD0;
		#if USE_UV2
			float2 UV2 :
			    TEXCOORD1;	

		#endif
	#endif
	float3 WorldNormal :
			    TEXCOORD2;
	float3 WorldPosition :
			    TEXCOORD3;
	float2 Depth : 
				TEXCOORD4;

};


static const float k0 = 0.00098, k1 = 0.9921, fUserMaxSPow = 0.2425;
static const float g_fMaxT = ( exp2(-10.0/sqrt( fUserMaxSPow )) - k0)/k1;
static const int nMipOffset = 0;

float GetSpecPowToMip(float fSpecPow, int nMips)
{	
	fSpecPow = 1 - pow(1 - fSpecPow, 8);	
	float fSmulMaxT = ( exp2(-10.0/sqrt( fSpecPow )) - k0)/k1;
	return float(nMips-1-nMipOffset)*(1.0 - clamp( fSmulMaxT/g_fMaxT, 0.0, 1.0 ));
}

float2 OctWrap( float2 v )
{
	return ( 1.0 - abs( v.yx ) ) * ( v.xy >= 0.0 ? 1.0 : -1.0 );
}

float2 Encode( float3 n )
{
	n /= ( abs( n.x ) + abs( n.y ) + abs( n.z ) );
	n.xy = n.z >= 0.0 ? n.xy : OctWrap( n.xy );
	n.xy = n.xy * 0.5 + 0.5;
	return n.xy;
}

float3 Decode( float2 encN )
{
	encN = encN * 2.0 - 1.0;
	float3 n;
	n.z = 1.0 - abs( encN.x ) - abs( encN.y );
	n.xy = n.z >= 0.0 ? encN.xy : OctWrap( encN.xy );
	n = normalize( n );
	return n;
}

struct PixelOutputType
{
float4 Target0 :
    SV_Target0; // albedo (rgb), metallic(a)
float4 Target1 : 
    SV_Target1; // normalFace(xy), normalPix(zw)
float4 Target2 : 
    SV_Target2; // Lightning (rgb), occlusion (a)
float4 Target3 :
    SV_Target3; // depth
float4 Target4 :
    SV_Target4; //cubemapSampleSpec (xyz), roughness (a)
};


PS_INPUT VS( VS_INPUT IN )
{
    PS_INPUT l_Output = (PS_INPUT)0;
    float4 lPos = float4( IN.Pos.xyz, 1.0 );

	float3 l_Normal = IN.Normal;
	#if USE_WEIGHTIDX
		float4 l_TempPos=float4(IN.pos.xyz, 1.0);
		l_Normal=float3(0,0,0);	
		lPos=mul(l_TempPos, m_Bones[l_Indices.x]) * IN.Weight.x;
		lPos+=mul(l_TempPos, m_Bones[l_Indices.y]) * IN.Weight.y;
		lPos+=mul(l_TempPos, m_Bones[l_Indices.z]) * IN.Weight.z;
		lPos+=mul(l_TempPos, m_Bones[l_Indices.w]) * IN.Weight.w;
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
	#endif
	l_Output.Pos = mul( lPos, m_World );
    l_Output.WorldPosition=l_Output.Pos.xyz;
    l_Output.Pos = mul( l_Output.Pos, m_View );
    l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.Depth=l_Output.Pos.zw;

    l_Output.WorldNormal = normalize(mul(normalize(l_Normal), (float3x3)m_World));
    #if USE_UV
	 	l_Output.UV = IN.UV;
	 	#if USE_UV2
	 		l_Output.UV2 = IN.UV2;
	 	#endif
    #endif

    #if USE_BUMP
     	l_Output.Tangent = normalize(mul(IN.Tangent.xyz, (float3x3)m_World));
    	l_Output.Binormal = normalize(mul(cross(IN.Tangent.xyz, IN.Normal.xyz), (float3x3)m_World));
    #endif
    return l_Output;
}



PixelOutputType PS(PS_INPUT IN) : SV_Target
{

   
    float g_SpecularExponent = 80.0;
    float g_SpecularContrib = 1.0;

    float3 pixelColor = m_RawData[0].xyz;
    float3 l_LAmbient = m_LightAmbient.xyz;

    float3 l_WorldPos = IN.WorldPosition;
    float3 l_Normal = normalize(IN.WorldNormal);

    float l_Depth=IN.Depth.x/IN.Depth.y;

    PixelOutputType l_Output = (PixelOutputType)0;
    #if USE_UV
		pixelColor = (DiffuseTexture.Sample(LinearSampler, IN.UV) * float4(pixelColor, 1.0)).xyz;
		#if USE_UV2
			float4 l_LightmapPixel = LightmapTexture.Sample(LightmapSampler, IN.UV2) * 2;
			//pixelColor = l_LightmapPixel.xyz * pixelColor;
			l_LAmbient = l_LightmapPixel.xyz;
		#endif
 	#endif
 	#if USE_BUMP
	 	float3 bump = m_RawData[1].x * (NormalMapTexture.Sample(NormalMapTextureSampler, IN.UV).rgb - float3(0.5, 0.5, 0.5));
	 	l_Normal = l_Normal + bump.x*IN.Tangent + bump.y*IN.Binormal;
    	l_Normal = normalize(l_Normal);
 	#endif

	l_Normal=Normal2Texture(l_Normal);


    l_Output.Target0 = float4(pixelColor, g_SpecularContrib);
    l_Output.Target1 = float4(l_LAmbient.xyz*pixelColor, g_SpecularExponent);
    l_Output.Target2 = float4(l_Normal, 0.0);	
    l_Output.Target3 = float4(l_Depth, l_Depth, l_Depth, 1.0);


    return l_Output;

/*
l_Out.Target0.a=m_MetallicFactor;
l_Out.Target1=float4(l_AmbientContrib.xyz, m_OcclusionFactor);
l_PlaneNormal.xy=Encode(l_PlaneNormal);
l_Normal.xy=Encode(l_Normal);
l_Out.Target2=float4(l_PlaneNormal.x, l_PlaneNormal.y, l_Normal.x, l_Normal.y);
float l_Depth=IN.Depth.z/IN.Depth.w;
l_Out.Target3=float4(l_Depth,l_Depth,l_Depth,1);
float3 l_EyeToWorldPosition=normalize(IN.WorldPosition-m_InverseView[3].xyz);
float3 l_ReflectVector=normalize(reflect(l_EyeToWorldPosition, l_Normal));
l_SpecularColor.xyz=l_EnvironmentTexture.SampleLevel(l_EnvironmentSampler, l_ReflectVector,
GetSpecPowToMip(m_RoughnessFactor, m_EnvironmentMipLevels)).xyz;
l_Out.Target4=float4(l_SpecularColor, m_RoughnessFactor);
*/ 
}
