#pragma once
#include "ParticleManager.h"
#include "ParticleSystemInstance.h"
#include "XML\XML.h"
#include "Utils/Logger.h"


CParticleManager::CParticleManager() {}

CParticleManager::~CParticleManager()
{
    Destroy();
}

bool CParticleManager::Load(const std::string &aFilename)
{

    bool lOk = false;
    CXMLDocument document;
    EXMLParseError error = document.LoadFile((aFilename).c_str());

    if (base::xml::SucceedLoad(error))
    {
        m_Filename = aFilename;
        CXMLElement* l_Meshes = document.FirstChildElement("particle_systems");

        if (l_Meshes)
        {
            for (tinyxml2::XMLElement* iMeshes = l_Meshes->FirstChildElement(); iMeshes != nullptr; iMeshes = iMeshes->NextSiblingElement())
            {
                CParticleSystemType *l_ParticleSystemType = new CParticleSystemType(iMeshes);
                if (!Add(l_ParticleSystemType->GetName(), l_ParticleSystemType))
                {
                    base::utils::CheckedDelete(l_ParticleSystemType);
                }
            }
        }
        return true;
    }

}


void CParticleManager::Reload()
{
    Destroy();
    Load(m_Filename);
}