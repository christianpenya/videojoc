#pragma once
#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_

#include "Light.h"

class CSpotLight : public CLight
{
public:
    CSpotLight(const CXMLElement* aElement);
    virtual ~CSpotLight();
    GET_SET(float, Angle);
    GET_SET(float, FallOff);
    void DrawImgui();
    virtual void SetShadowMap(CRenderManager &RenderManager);
private:
    float m_Angle;
    float m_FallOff;
};

#endif