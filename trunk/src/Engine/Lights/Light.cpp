#pragma once
#include "Light.h"
#include "Utils\EnumToString.h"

CLight::~CLight()
{
}


CLight::CLight(ELightType aLightType)
{
    m_Type = aLightType;
}


CLight::CLight(const CXMLElement* aElement)
    : CSceneNode(aElement)
    , m_Name(aElement->GetAttribute<std::string>("name", ""))
    , m_Intensity ( aElement->GetAttribute<float>("intensity", 1.0f) )
    , m_SpecularIntensity(aElement->GetAttribute<float>("specular_intensity", 1.0f) )
    , m_Color(aElement->GetAttribute<CColor>("color", CColor(1, 1, 1)) )
    , m_RangeAttenuation(aElement->GetAttribute<Vect2f>("attenuation_range", Vect2f(0.0f,100.0f)))
    , m_Enabled( aElement->GetAttribute<bool>("enabled", true))
{
    bool lOk = (EnumString<ELightType>::ToEnum(m_Type, aElement->GetAttribute<std::string>("type", "")));
    m_Position = aElement->GetAttribute<Vect3f>("position", Vect3f(0.0f, 0.0f, 0.0f));
    assert(lOk&&"This kind of light does not exist!!");
}



