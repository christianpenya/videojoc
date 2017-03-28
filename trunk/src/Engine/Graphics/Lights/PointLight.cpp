#include "PointLight.h"

CPointLight::~CPointLight()
{
}


CPointLight::CPointLight(const CXMLElement* aElement) : CLight(aElement)
{

}

void CPointLight::SetShadowMap(CRenderManager &RenderManager) {}