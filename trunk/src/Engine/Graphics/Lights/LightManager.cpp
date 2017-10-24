#pragma once
#include "LightManager.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Engine\engine.h"
#include "Graphics/Buffers/ConstantBufferManager.h"
#include "XML\xml.h"
#include <set>
#include <cassert>
#include "Utils/CheckedRelease.h"

CLightManager::CLightManager() {}
CLightManager::~CLightManager()
{
    //base::utils::CTemplatedMapVector<CLight>::Clear();

    /*
    for (std::vector<CLight *>::iterator it = m_ResourcesVector.begin(); it != m_ResourcesVector.end(); ++it)
    {
        __H_CHECKED_RELEASE__(*it);
    }
    */
}

bool CLightManager::Load(const std::string& aFilename, bool update)
{
    m_LevelLightsFilename = aFilename;
    return Load(update);
}

bool CLightManager::Load(bool update)
{
    CXMLDocument document;
    CLight::ELightType lLightType;

    if (m_LevelLightsFilename.find(".xml") == std::string::npos)
    {
        m_LevelLightsFilename = "data/scenes/" + m_LevelLightsFilename + "/lights_" + m_LevelLightsFilename + ".xml";
    }

    EXMLParseError error = document.LoadFile((m_LevelLightsFilename).c_str());
    std::set< std::string > lNamesLightsFromXML;

    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lLights = document.FirstChildElement("lights");

        if (lLights)
        {
            for (tinyxml2::XMLElement *iLight = lLights->FirstChildElement(); iLight != nullptr; iLight = iLight->NextSiblingElement())
            {
                if (strcmp(iLight->Name(), "light") == 0)
                {
                    EnumString<CLight::ELightType>::ToEnum(lLightType, iLight->GetAttribute<std::string>("type", ""));
                    const std::string lLightName = iLight->GetAttribute<std::string>("name", "null name");
                    assert(lNamesLightsFromXML.count(lLightName) == 0);
                    lNamesLightsFromXML.insert(lLightName);
                    CLight* lLight = (*this)(lLightName);

                    if ( lLight )
                    {
                        lLight->Initialize(iLight);
                    }
                    else
                    {
                        if (lLightType == CLight::ePoint)
                        {
                            lLight = new CPointLight(iLight);
                        }
                        else if (lLightType == CLight::eSpot)
                        {
                            lLight = new CSpotLight(iLight);
                        }
                        else  if (lLightType == CLight::eDirectional)
                        {
                            lLight = new CDirectionalLight(iLight);
                        }

                        Add(lLight->GetName(), lLight);
                    }
                }
            }
        }
    }

    if (update)
    {
        std::set<std::string> lMissingLights;
        for (std::vector<CLight *>::iterator it = m_ResourcesVector.begin(); it != m_ResourcesVector.end(); ++it)
        {
            if (lNamesLightsFromXML.find((*it)->GetName()) == lNamesLightsFromXML.end())
            {
                assert(lMissingLights.count((*it)->GetName()) == 0);
                lMissingLights.insert((*it)->GetName());
            }
        }

        for (std::set<std::string>::iterator iMissingLight = lMissingLights.begin(); iMissingLight != lMissingLights.end(); ++iMissingLight)
        {
            Remove(*iMissingLight);
        }
    }

    return true;
}

void CLightManager::SetLightConstants(size_t idLight, CLight* alight)
{
    CConstantBufferManager& lConstanBufferManager = CEngine::GetInstance().GetConstantBufferManager();
    CRenderManager& lRM = CEngine::GetInstance().GetRenderManager();

    lConstanBufferManager.mLightsDesc.m_LightType[idLight] = alight->GetLightType();
    lConstanBufferManager.mLightsDesc.m_LightColor[idLight] = alight->GetColor();
    lConstanBufferManager.mLightsDesc.m_LightEnabled[idLight] = alight->IsVisible();
    lConstanBufferManager.mLightsDesc.m_LightIntensity[idLight] = alight->GetIntensity();
    lConstanBufferManager.mLightsDesc.m_LightAttenuationStartRange[idLight] = alight->GetRangeAttenuation().x;
    lConstanBufferManager.mLightsDesc.m_LightAttenuationEndRange[idLight] = alight->GetRangeAttenuation().y;
    lConstanBufferManager.mLightsDesc.m_LightDirection[idLight] = Vect4f(alight->GetForward(), 0.0f);// Vect4f(alight->GetPitch(), alight->GetYaw(), alight->GetRoll(), 0.0f);
    lConstanBufferManager.mLightsDesc.m_LightPosition[idLight] = Vect4f(alight->GetPosition(), 0.0f);
    lConstanBufferManager.mLightsDesc.m_LightAmbient = 0.0f;
    if (alight->GetLightType() == CLight::eSpot) //Spot
    {
        lConstanBufferManager.mLightsDesc.m_LightFallOffAngle[idLight] = ((CSpotLight *)alight)->GetFallOff();
        lConstanBufferManager.mLightsDesc.m_LightAngle[idLight] = ((CSpotLight *)alight)->GetAngle();
    }

    if (alight->GetGenerateShadowMap())
    {
        CDynamicTexture *l_ShadowMap = alight->GetShadowMap();
        CTexture *l_ShadowMask = alight->GetShadowMaskTexture();
        lConstanBufferManager.mLightsDesc.m_UseShadowMap[idLight] = 1.0f;
        lConstanBufferManager.mLightsDesc.m_UseShadowMask[idLight] = l_ShadowMask != NULL ? 1.0f : 0.0f;
        lConstanBufferManager.mLightsDesc.m_ShadowMapBias[idLight] = 0.0f;
        lConstanBufferManager.mLightsDesc.m_ShadowMapStrength[idLight] = 1.0f;
        lConstanBufferManager.mLightsDesc.m_LightView[idLight] = alight->GetViewShadowMap();
        lConstanBufferManager.mLightsDesc.m_LightProjection[idLight] = alight->GetProjectionShadowMap();

        // TODO cuidad, numero de sampler para shadowmap Hardcoded
        l_ShadowMap->Bind(5, lRM.GetDeviceContext());

        if (l_ShadowMask != NULL)
            l_ShadowMask->Bind(5 + 1, lRM.GetDeviceContext());
    }
    else
    {
        lConstanBufferManager.mLightsDesc.m_UseShadowMap[idLight] = 0.0f;
    }

    lConstanBufferManager.BindBuffer(lRM.GetDeviceContext(), CConstantBufferManager::CB_Light);
}

void CLightManager::SetLightsConstants()
{
    for (TMapResources::iterator iLightMapEntry = m_ResourcesMap.begin(); iLightMapEntry != m_ResourcesMap.end(); ++iLightMapEntry)
    {
        CLight* lLight = iLightMapEntry->second.m_Value;
        SetLightConstants(iLightMapEntry->second.m_Id, lLight);
    }
}

void CLightManager::DrawImgui()
{
    if (ImGui::TreeNode("Lights"))
    {
        ImGui::ColorEditMode(ImGuiColorEditMode_RGB);

        for (TMapResources::iterator iLightMapEntry = m_ResourcesMap.begin(); iLightMapEntry != m_ResourcesMap.end(); ++iLightMapEntry)
        {
            CLight* lLight = iLightMapEntry->second.m_Value;
            ImGui::PushID(iLightMapEntry->second.m_Id);
            lLight->DrawImgui();
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
}

CLight* CLightManager::GetLightByIdx(size_t idLight)
{
    return m_ResourcesVector[idLight];
}
