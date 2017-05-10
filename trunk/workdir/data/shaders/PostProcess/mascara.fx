#include "globals.fx"
#include "Samplers.fxh"

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float2 UV :
    TEXCOORD0;
};


float4 PS(PS_INPUT IN) : SV_Target
{
//	return float4(1.0,1.0,0.0,1.0);
    float4 l_ColorVista = T0Texture.Sample(S0Sampler, IN.UV) * float4(0.0,0.0,0.0,1.0);
    float4 l_Color = T1Texture.Sample(S1Sampler, IN.UV) * float4(1.0,1.0,1.0,1.0);
	
    return (l_ColorVista+l_Color);
}
