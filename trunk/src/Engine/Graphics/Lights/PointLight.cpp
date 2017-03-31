#include "PointLight.h"

CPointLight::~CPointLight()
{
}


CPointLight::CPointLight(CXMLElement* aElement) : CLight(aElement)
{

}

void CPointLight::SetShadowMap(CRenderManager &RenderManager) {}