#pragma once
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Utils\TemplatedMapVector.h"
#include "Math\Color.h"
#include "Utils\StringUtils.h"
#include "Scenes\SceneNode.h"
#include "Utils\EnumToString.h"

class CLight : public CSceneNode
{
public:
    enum ELightType
    {
        ePoint = 0,
        eSpot = 1,
        eDirectional =2
    };

public:
    CLight(ELightType aLightType);
    //CLight(const CXMLElement* aElement, ELightType aLightType);
    CLight(const CXMLElement* aElement);
    virtual ~CLight();
    GET_SET(ELightType, Type);
    GET_SET(std::string, Name);
    GET_SET(float, Intensity);
    GET_SET(float, SpecularIntensity);
    GET_SET_REF(CColor, Color);
    GET_SET_REF(Vect2f, RangeAttenuation);
    void DrawImgui();
protected:
    ELightType m_Type;
    float m_Intensity;
    float m_SpecularIntensity;
    CColor m_Color;
    Vect2f m_RangeAttenuation;
    std::string m_Name;
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