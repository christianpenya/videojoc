#pragma once
#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

#include "Light.h"
#include "Utils\StringUtils.h"
#include "XML\xml.h"

class CPointLight : public CLight
{
public:
    CPointLight();
    CPointLight(const CXMLElement* aElement);
    virtual ~CPointLight();
};


#endif