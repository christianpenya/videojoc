#pragma once
#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_

#include "Light.h"

class CSpotLight : public CLight
{
public:
    CSpotLight(CXMLElement* aElement);
    virtual ~CSpotLight();
    GET_SET(float, Angle);
    GET_SET(float, FallOff);
    void DrawImgui();
    virtual void SetShadowMap(CRenderManager &RenderManager);
    virtual void Initialize(CXMLElement* aElement);
private:
    float m_Angle;
    float m_FallOff;
};

#endif