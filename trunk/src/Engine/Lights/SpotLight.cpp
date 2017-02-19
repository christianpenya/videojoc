#include "SpotLight.h"
#include "ImGui/imgui.h"

CSpotLight::~CSpotLight()
{
}

CSpotLight::CSpotLight(const CXMLElement* aElement)
    : CLight(aElement)
    , m_Angle(aElement->GetAttribute<float>("angle",45.0f))
    , m_FallOff(aElement->GetAttribute<float>("fall_off",45.0f))
{


}

void CSpotLight::DrawImgui()
{
    ImGui::SliderFloat("Fall Off Angle",&m_FallOff, 0.0f, 360.0f);
    ImGui::SliderFloat("Angle", &m_Angle, 0.0f, 360.0f);
}
