#pragma once
#include "LaserManager.h"
#include "Laser.h"
#include "XML\XML.h"
#include "Utils/Logger.h"

CLaserManager::CLaserManager() {}

CLaserManager::~CLaserManager()
{
    //    CTemplatedMap<CNavMesh>::Destroy();
}

bool CLaserManager::Load(const std::string &aFilename)
{
    bool lOk = false;
    CXMLDocument document;
    EXMLParseError error = document.LoadFile((aFilename).c_str());

    if (base::xml::SucceedLoad(error))
    {
        m_Filename = aFilename;
        CXMLElement* l_Meshes = document.FirstChildElement("lasers");

        if (l_Meshes)
        {
            for (tinyxml2::XMLElement* iMeshes = l_Meshes->FirstChildElement(); iMeshes != nullptr; iMeshes = iMeshes->NextSiblingElement())
            {
                CLaser *l_Laser = new CLaser(iMeshes);
                if (!Add(l_Laser->GetName(), l_Laser))
                {
                    base::utils::CheckedDelete(l_Laser);
                }
            }
        }
        return true;
    }
}

void CLaserManager::Reload()
{
    Destroy();
    Load(m_Filename);
}
