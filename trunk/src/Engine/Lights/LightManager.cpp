#pragma once
#include "LightManager.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

CLightManager::CLightManager()
{
}

CLightManager::~CLightManager()
{
    Destroy();
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


bool CLightManager::ReLoad()
{
    Clear();
    return Load();
}