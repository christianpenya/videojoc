#pragma once
#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_

#include "Light.h"
#include "XML\xml.h"


class CDirectionalLight : public CLight
{
public:
    CDirectionalLight();
    CDirectionalLight(const CXMLElement* aElement);
    virtual ~CDirectionalLight();
};

#endif