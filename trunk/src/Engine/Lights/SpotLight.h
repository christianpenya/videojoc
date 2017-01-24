#pragma once
#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_

#include "Light.h"
#include "Utils\StringUtils.h"
#include "XML\xml.h"

class CSpotLight : public CLight
{
public:
    CSpotLight();
    CSpotLight(const CXMLElement* aElement);
    virtual ~CSpotLight();
    GET_SET(float, Angle);
    GET_SET(float, FallOff);
private:
    float m_Angle;
    float m_FallOff;
};

#endif