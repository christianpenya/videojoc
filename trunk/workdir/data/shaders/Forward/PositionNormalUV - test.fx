#include "Colors.fxh"
#include "globals.fxh"

Texture2D DiffuseTexture :
register(t0);
SamplerState LinearSampler :
register(s0);

struct VS_INPUT
{
float3 Pos :
    POSITION;
float3 Normal :
    NORMAL;
float2 UV :
    TEXCOORD0;
};
struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float3 Normal :
    NORMAL;
float2 UV :
    TEXCOORD0;
float3 WorldNormal :
    TEXCOORD1;
float3 WorldPosition :
    TEXCOORD2;
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

    l_Output.UV = IN.UV;

    return l_Output;
}

float4 PS( PS_INPUT IN ) : SV_Target
{
    float3 NormalPixel = normalize(IN.Normal);


    float3 WorldPos = IN.WorldPosition;
    float4 ColorPixel = DiffuseTexture.Sample(LinearSampler, IN.UV);

    float3 l_LAmbient = m_LightAmbient.xyz * ColorPixel.xyz;
    return float4(ColorPixel.xyz, 1.0);
    /*float3 DiffuseColor = float3(0.0, 0.0, 0.0);
    float3 SpecularColor = float3(0.0, 0.0, 0.0);

    float3 l_LDiffuseSpecular = float3(0.0, 0.0, 0.0);
    float3 l_LDiffuseSpecularTmp = float3(0.0, 0.0, 0.0);

    for (int IdLight = 0; IdLight<MAX_LIGHTS_BY_SHADER; IdLight++)
    {
        //CalculateSingleLight(i//IdLight, l_Normal//NormalPixel, WorldPos, l_DiffuseColor//ColorPixel, l_DiffuseTmp//DiffuseColor, l_SpecularTmp//SpecularColor);

        if (m_LightEnabledArray[IdLight] == 1)
        {

            float g_SpecularExponent = 80.0;
            float g_SpecularContrib = 1.0;

            float3 l_LightDirection = m_LightDirection[IdLight];
            float l_DistanceAtten = 1.0;
            float l_AngleAtenuation = 1.0;

            // OMNI
            if (m_LightTypeArray[IdLight] == 0)
            {
                float l_DistanceToPixel = length(WorldPos - m_LightPosition[IdLight]);
                l_LightDirection = (WorldPos - m_LightPosition[IdLight]) / l_DistanceToPixel;
                l_DistanceAtten = 1.0 - saturate((l_DistanceToPixel - m_LightAttenuationStartRangeArray[IdLight]) / (m_LightAttenuationEndRangeArray[IdLight] - m_LightAttenuationStartRangeArray[IdLight]));

            }
            //SPOT
            else if (m_LightTypeArray[IdLight] == 1)
            {
                float l_DistanceToPixel = length(WorldPos - m_LightPosition[IdLight]);
                l_LightDirection = (WorldPos - m_LightPosition[IdLight]) / l_DistanceToPixel;
                l_DistanceAtten = 1.0 - saturate((l_DistanceToPixel - m_LightAttenuationStartRangeArray[IdLight]) / (m_LightAttenuationEndRangeArray[IdLight] - m_LightAttenuationStartRangeArray[IdLight]));

                float l_SpotAngle = cos(m_LightAngleArray[IdLight] * 0.5*(3.1416 / 180.0));
                float l_SpotFallOff = cos(m_LightFallOffAngleArray[IdLight] * 0.5*(3.1416 / 180.0));
                float l_DotAngle = dot(l_LightDirection, m_LightDirection[IdLight]);

                float l_AngleAtenuation = saturate((l_DotAngle - l_SpotFallOff) / (l_SpotAngle - l_SpotFallOff));
            }
            // No hacen falta calculos adicionales para la luz direccional

            float3 l_Eye = m_CameraPosition.xyz;
            float3 l_ViewDir = normalize(l_Eye - WorldPos);
            float3 Hn = normalize(l_ViewDir - l_LightDirection.xyz);

            float l_DiffuseContrib = saturate(dot(-l_LightDirection, NormalPixel));
            float l_SpecularContrib = pow(saturate(dot(Hn, NormalPixel)), 20.0);

            DiffuseColor = l_DiffuseContrib * m_LightIntensity[IdLight] * m_LightColor[IdLight].xyz * ColorPixel.xyz * l_DistanceAtten * l_AngleAtenuation;
            SpecularColor = l_SpecularContrib* m_LightIntensity[IdLight] * m_LightColor[IdLight].xyz *g_SpecularContrib * l_DistanceAtten * l_AngleAtenuation;
        }

        l_LDiffuseSpecularTmp = DiffuseColor + SpecularColor;
        l_LDiffuseSpecular = l_LDiffuseSpecular + l_LDiffuseSpecularTmp;

        DiffuseColor = float3(0.0, 0.0, 0.0);
        SpecularColor = float3(0.0, 0.0, 0.0);
    }

    return float4(l_LAmbient + l_LDiffuseSpecular, 1.0);*/
}


