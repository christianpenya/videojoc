#pragma once
#include "LightManager.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Engine\engine.h"
#include "Graphics/Buffers/ConstantBufferManager.h"
#include "XML\xml.h"

CLightManager::CLightManager() {}

CLightManager::~CLightManager()
{
    CTemplatedMapVector<CLight>::Destroy();
}

bool CLightManager::Load(const std::string& aFilename)
{
    m_LevelLightsFilename = aFilename;
    return Load();
}

bool CLightManager::Load()
{
    bool lOk = false;
    CXMLDocument document;
    EXMLParseError error = document.LoadFile((m_LevelLightsFilename).c_str());
    CLight::ELightType lLightType;

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

                    if (lLightType == 0) //Point
                    {
                        CPointLight* lLight = new CPointLight(iLight);
                        Add(lLight->GetName(), lLight);
                    }
                    else if (lLightType == 1) //Spot
                    {
                        CSpotLight* lLight = new CSpotLight(iLight);
                        Add(lLight->GetName(), lLight);
                    }
                    else  //Directional
                    {
                        CDirectionalLight* lLight = new CDirectionalLight(iLight);
                        Add(lLight->GetName(), lLight);
                    }

                }
            }

            lOk=true;
        }
    }

    return lOk;
}

void CLightManager::SetLightConstants(size_t idLight, CLight* alight)
{
    CConstantBufferManager& lConstanBufferManager = CEngine::GetInstance().GetConstantBufferManager();
    CRenderManager& lRM = CEngine::GetInstance().GetRenderManager();

    lConstanBufferManager.mLightsDesc.m_LightType[idLight] = alight->GetType();
    lConstanBufferManager.mLightsDesc.m_LightColor[idLight] = alight->GetColor();
    lConstanBufferManager.mLightsDesc.m_LightEnabled[idLight] = alight->IsVisible();
    lConstanBufferManager.mLightsDesc.m_LightIntensity[idLight] = alight->GetIntensity();
    lConstanBufferManager.mLightsDesc.m_LightAttenuationStartRange[idLight] = alight->GetRangeAttenuation().x;
    lConstanBufferManager.mLightsDesc.m_LightAttenuationEndRange[idLight] = alight->GetRangeAttenuation().y;
    lConstanBufferManager.mLightsDesc.m_LightDirection[idLight] = Vect3f(alight->GetPosition().x, alight->GetPosition().y, alight->GetPosition().z);
    lConstanBufferManager.mLightsDesc.m_LightPosition[idLight] = Vect3f(alight->GetPrevPosition().x, alight->GetPrevPosition().y, alight->GetPrevPosition().z);
    lConstanBufferManager.mLightsDesc.m_LightAmbient = 0.25f;
    //lConstanBufferManager.mLightsDesc.m_LightAmbient = alight->GetSpecularIntensity();
    //lConstanBufferManager.mLightsDesc.m_LightFallOffAngle[idLight] = ((CSpotLight *)alight)->GetFallOff();
    //lConstanBufferManager.mLightsDesc.m_LightAngle[idLight] = ((CSpotLight *)alight)->GetAngle();
    if (alight->GetType() == 1) //Spot
    {
        lConstanBufferManager.mLightsDesc.m_LightFallOffAngle[idLight] = ((CSpotLight *)alight)->GetFallOff();
        lConstanBufferManager.mLightsDesc.m_LightAngle[idLight] = ((CSpotLight *)alight)->GetAngle();
    }
    //lConstanBufferManager.BindVSBuffer(lRM.GetDeviceContext(), CConstantBufferManager::CB_LightVS);
    //lConstanBufferManager.BindPSBuffer(lRM.GetDeviceContext(), CConstantBufferManager::CB_LightPS);
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

bool CLightManager::ReLoad()
{
    Clear();
    return Load();
}

void CLightManager::DrawImgui()
{
    if (ImGui::CollapsingHeader("Lights"))
    {
        ImGui::BeginChild("#lights", ImVec2(400, 400), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::PushItemWidth(-130);
        ImGui::ColorEditMode(ImGuiColorEditMode_RGB);

        for (TMapResources::iterator iLightMapEntry = m_ResourcesMap.begin(); iLightMapEntry != m_ResourcesMap.end(); ++iLightMapEntry)
        {
            CLight* lLight = iLightMapEntry->second.m_Value;
            ImGui::PushID(iLightMapEntry->second.m_Id);
            lLight->DrawImgui();
            ImGui::PopID();
        }
        ImGui::PopItemWidth();
        ImGui::EndChild();
    }
}

CLight* CLightManager::GetLightByIdx(size_t idLight)
{
    return m_ResourcesVector[idLight];
}

