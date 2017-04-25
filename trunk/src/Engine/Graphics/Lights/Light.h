#pragma once
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Utils/TemplatedMapVector.h"
#include "Math/Color.h"
#include "Utils/StringUtils.h"
#include "Graphics/Scenes/SceneNode.h"
#include "Utils/EnumToString.h"
#include "Render/RenderPipeline/DynamicTexture.h"
#include "Graphics/Scenes/Layer.h"

class CLight : public CSceneNode
{
public:
    enum ELightType
    {
        ePoint = 0,
        eSpot = 1,
        eDirectional =2
    };

    CLight(ELightType aLightType);
    CLight(CXMLElement* aElement);
    virtual ~CLight();

    GET_SET(ELightType, LightType);
    GET_SET(std::string, Name);
    GET_SET(float, Intensity);
    GET_SET(float, SpecularIntensity);
    GET_SET(std::vector<std::string>, LayerNames);
    GET_SET_REF(CColor, Color);
    GET_SET_REF(Vect2f, RangeAttenuation);
    GET_SET_REF(Mat44f, ViewShadowMap);
    GET_SET_REF(Mat44f, ProjectionShadowMap);
    GET_SET_PTR(CDynamicTexture, ShadowMap);
    GET_SET_PTR(CTexture, ShadowMaskTexture);

    void DrawImgui();
    virtual void SetShadowMap(CRenderManager &RenderManager) = 0;
    bool GetGenerateShadowMap();

protected:
    ELightType m_LightType;
    float m_Intensity;
    float m_SpecularIntensity;
    CColor m_Color;
    Vect2f m_RangeAttenuation;
    std::string m_Name;

    bool m_GenerateShadowMap;
    CDynamicTexture *m_pShadowMap;
    CTexture *m_pShadowMaskTexture;
    std::vector<std::string> m_LayerNames;
    Mat44f m_ViewShadowMap, m_ProjectionShadowMap;
};

Begin_Enum_String(CLight::ELightType)
{
    Enum_String_Id(CLight::ePoint, "point");
    Enum_String_Id(CLight::eSpot, "spot");
    Enum_String_Id(CLight::eDirectional, "directional");
    Enum_String_Id(CLight::eSpot, "#freeSpot");
    Enum_String_Id(CLight::eDirectional, "#freeDirect");
}
End_Enum_String;

#endif
