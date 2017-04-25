#include "PointLight.h"

CPointLight::~CPointLight()
{
}

CPointLight::CPointLight(CXMLElement* aElement) : CLight(aElement)
{
    m_LightType = CLight::ePoint;
}

void CPointLight::SetShadowMap(CRenderManager &RenderManager) {}