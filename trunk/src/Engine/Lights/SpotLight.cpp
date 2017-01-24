#include "SpotLight.h"

CSpotLight::~CSpotLight()
{
}

CSpotLight::CSpotLight(const CXMLElement* aElement)
    : CLight(aElement)
    , m_Angle(aElement->GetAttribute<float>("angle",45.0f))
    , m_FallOff(aElement->GetAttribute<float>("fall_off",45.0f))
{


}

