#pragma once
#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

#include "Light.h"

class CPointLight : public CLight
{
public:
    CPointLight(const CXMLElement* aElement);
    virtual ~CPointLight();
};


#endif