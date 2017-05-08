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
    m_GenerateShadowMap(false),
    m_pShadowMap(nullptr),
    m_pShadowMaskTexture(nullptr),
    m_LightType(aLightType)
{
}

CLight::CLight(CXMLElement* aElement)
    : CSceneNode(aElement)
    , m_Intensity(aElement->GetAttribute<float>("intensity", 1.0f))
    , m_SpecularIntensity(aElement->GetAttribute<float>("specular_intensity", 1.0f))
    , m_Color(aElement->GetAttribute<CColor>("color", CColor(1, 1, 1)))
    , m_RangeAttenuation(aElement->GetAttribute<Vect2f>("attenuation_range", Vect2f(0.0f, 100.0f)))
    , m_Name(aElement->GetAttribute<std::string>("name", ""))
    , m_GenerateShadowMap(aElement->GetAttribute<bool>("generate_shadow_map", false))
    , m_pShadowMaskTexture(nullptr)
{
    bool lOk = (EnumString<ELightType>::ToEnum(m_LightType, aElement->GetAttribute<std::string>("type", "")));
    m_Visible = aElement->GetAttribute<bool>("active", true);

    tinyxml2::XMLElement const* iTransformLight = aElement->FirstChildElement();
    CTransform::SetForward(iTransformLight->GetAttribute<Vect3f>("forward", Vect3f(0.0f, -1.0f, 0.0f)));

    CTextureManager& lTextureManager = CEngine::GetInstance().GetTextureManager();
    //TODO
    //CTexture* l_Texture = lTextureManager.GetTexture(aElement->GetAttribute<std::string>("shadow_texture_mask", ""));
    //m_pShadowMaskTexture->SetTexture(l_Texture->GetTexture());
    assert(lOk && "This kind of light does not exist!!");

    // SHADOWMAP
    if (m_GenerateShadowMap)
    {
        m_pShadowMap = new CDynamicTexture(m_Name, Vect2u(aElement->GetAttribute<uint32>("shadow_map_width", 128), aElement->GetAttribute<uint32>("shadow_map_height", 128)));
        CEngine::GetInstance().GetTextureManager().AddTexture(*m_pShadowMap);
        //TODO ShadowTextureMask shadow_texture_mask

        for (tinyxml2::XMLElement *lLayerNode = aElement->FirstChildElement(); lLayerNode != nullptr; lLayerNode = lLayerNode->NextSiblingElement())
        {
            if (strcmp(lLayerNode->Name(), "layer") == 0)
            {
                //leemos textura
                //TODO: creamos layer nueva, no deberia ser necesario.. nos valdría con el nombre de la escena?
                std::string lLayer = lLayerNode->GetAttribute<std::string>("name", "");
                m_LayerNames.push_back(lLayer);
            }
        }
    }
}

void CLight::DrawImgui()
{
    if (ImGui::CollapsingHeader(m_Name.c_str()))
    {
        ImGui::ColorEdit4("Color", (float*)&m_Color, true);
        ImGui::SliderFloat("Intensity", &m_Intensity, 0.25f, 1.0f);
        ImGui::SliderFloat2("Attenuation Range", (float*)&m_RangeAttenuation, 0.25f, 100.0f);
        ImGui::SliderFloat3("Position", (float*)&m_Position, -100.0f, 100.0f);
        ImGui::SliderFloat("Yaw", (float*)&m_Yaw, -6.28f, 6.28f);
        ImGui::SliderFloat("Pitch", (float*)&m_Pitch, -6.28f, 6.28f);
        ImGui::SliderFloat("Roll", (float*)&m_Roll, -6.28f, 6.28f);
        ImGui::Checkbox("Visible", &m_Visible);
        if (m_LightType == 1) //Spot
            ((CSpotLight *)this)->DrawImgui();
    }
}

bool CLight::GetGenerateShadowMap()
{
    return m_GenerateShadowMap;
}
