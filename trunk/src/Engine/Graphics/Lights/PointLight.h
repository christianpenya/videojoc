#pragma once
#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

#include "Light.h"

class CPointLight : public CLight
{
public:
    CPointLight(CXMLElement* aElement);
    virtual ~CPointLight();
    virtual void SetShadowMap(CRenderManager &RenderManager);
    virtual void Initialize(CXMLElement* aElement);
};


#endif