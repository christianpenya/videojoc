#pragma once
#include "Light.h"
#include "Utils\EnumToString.h"
#include "ImGUI\imgui.h"
#include "SpotLight.h"
#include "XML\XML.h"
#include "Engine\Engine.h"
#include "Graphics\Textures\TextureManager.h"

CLight::~CLight() {}

CLight::CLight(ELightType aLightType):
    m_Intensity(0),
    m_SpecularIntensity(0),
    m_Type(aLightType)
{}

CLight::CLight(const CXMLElement* aElement)
    : CSceneNode(aElement)
    , m_Intensity ( aElement->GetAttribute<float>("intensity", 1.0f) )
    , m_SpecularIntensity(aElement->GetAttribute<float>("specular_intensity", 1.0f) )
    , m_Color(aElement->GetAttribute<CColor>("color", CColor(1, 1, 1)) )
    , m_RangeAttenuation(aElement->GetAttribute<Vect2f>("attenuation_range", Vect2f(0.0f,100.0f)))
    , m_Name(aElement->GetAttribute<std::string>("name", ""))
    , m_GenerateShadowMap(aElement->GetAttribute<bool>("generate_shadow_map", false))

{
    bool lOk = (EnumString<ELightType>::ToEnum(m_Type, aElement->GetAttribute<std::string>("type", "")));
    m_Visible = aElement->GetAttribute<bool>("enabled", true);

    tinyxml2::XMLElement const *iTransformLight = aElement->FirstChildElement();
    m_Position = iTransformLight->GetAttribute<Vect3f>("position", Vect3f(0.0f, 0.0f, 0.0f));
    m_PrevPos = iTransformLight->GetAttribute<Vect3f>("forward", Vect3f(0.0f, 0.0f, 1.0f));

    CTextureManager& lTextureManager = CEngine::GetInstance().GetTextureManager();
    //CTexture* l_Texture = lTextureManager.GetTexture(aElement->GetAttribute<std::string>("shadow_texture_mask", ""));
    //m_pShadowMaskTexture->SetTexture(l_Texture->GetTexture());
    assert(lOk && "This kind of light does not exist!!");
}

void CLight::DrawImgui()
{
    if (ImGui::CollapsingHeader(m_Name.c_str()))
    {
        ImGui::ColorEdit4("Color", (float*)&m_Color, true);
        ImGui::SliderFloat("Intensity", &m_Intensity, 0.25f, 1.0f);
        ImGui::SliderFloat2("Attenuation Range", (float*)&m_RangeAttenuation, 0.25f, 100.0f);
        ImGui::SliderFloat3("Position", (float*)&m_Position, -100.0f, 100.0f);
        ImGui::SliderFloat3("Forward", (float*)&m_PrevPos, -100.0f, 100.0f);
        ImGui::Checkbox("Visible", &m_Visible);
        if (m_Type == 1) //Spot
            ((CSpotLight *)this)->DrawImgui();
    }
}
