#pragma once
#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_

#include "Light.h"

class CDirectionalLight : public CLight
{
public:
    CDirectionalLight(const CXMLElement* aElement);
    virtual ~CDirectionalLight();
    virtual void SetShadowMap(CRenderManager &RenderManager);
protected:
    Vect2f m_OrthoShadowMapSize;

};

#endif