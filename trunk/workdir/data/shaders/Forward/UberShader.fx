#include "globals.fx"

#if USE_UV
	Texture2D DiffuseTexture : 	register(t0);
	SamplerState LinearSampler : register(s0);
#endif

struct VS_INPUT
{
float3 Pos : POSITION;
float3 Normal :NORMAL;


};
struct PS_INPUT
{
float4 Pos : SV_POSITION;
float3 Normal : NORMAL;
	
};

PS_INPUT VS( VS_INPUT IN )
{
    PS_INPUT l_Output = (PS_INPUT)0;
    float4 lPos = float4( IN.Pos.xyz, 1.0 );
    l_Output.Pos = mul( lPos, m_World );
    l_Output.Pos = mul( l_Output.Pos, m_View );
    l_Output.Pos = mul( l_Output.Pos, m_Projection );

    l_Output.Normal=normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	
    return l_Output;
}

float4 PS( PS_INPUT IN ) : SV_Target
{
    float3 l_Normal = normalize(IN.Normal);
    float3 l_WorldPos = IN.Pos.xyz;
    float4 l_DiffuseColor = float4(m_RawData[0].xyz, 1.0);

    float3 l_DiffuseTmp =	float3(0.0, 0.0, 0.0);
    float3 l_SpecularTmp =	float3(0.0, 0.0, 0.0);
	float3 l_LDiffuseSpecular = float3(0.0, 0.0, 0.0);

	
	float3 l_LAmbient = m_LightAmbient.xyz * l_DiffuseColor.xyz;

    for(int i = 0; i < 2; i++)
    {
        l_DiffuseTmp =	float3(0.0, 0.0, 0.0);
        l_SpecularTmp =	float3(0.0, 0.0, 0.0);
		
		CalculateSingleLight(i, l_Normal, l_WorldPos, l_DiffuseColor,l_DiffuseTmp, l_SpecularTmp);

        l_LDiffuseSpecular += l_DiffuseTmp + l_SpecularTmp;
    }

    return float4(l_LAmbient+l_LDiffuseSpecular,1.0);
}
