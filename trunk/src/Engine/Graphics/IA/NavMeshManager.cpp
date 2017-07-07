#pragma once
#include "NavMeshManager.h"
#include "NavMesh.h"
#include "XML\XML.h"
#include "Utils/Logger.h"

CNavMeshManager::CNavMeshManager() {}

CNavMeshManager::~CNavMeshManager()
{
    CTemplatedMap<CNavMesh>::Destroy();
}

bool CNavMeshManager::Load(const std::string &aFilename)
{
    bool lOk = false;
    CXMLDocument document;
    EXMLParseError error = document.LoadFile((aFilename).c_str());

    if (base::xml::SucceedLoad(error))
    {
        m_Filename = aFilename;
        CXMLElement* l_Meshes = document.FirstChildElement("navmeshs");

        if (l_Meshes)
        {
            for (tinyxml2::XMLElement* iMeshes = l_Meshes->FirstChildElement(); iMeshes != nullptr; iMeshes = iMeshes->NextSiblingElement())
            {
                CNavMesh *l_NavMesh = new CNavMesh(iMeshes);
                if (!Add(l_NavMesh->GetName(), l_NavMesh))
                {
                    base::utils::CheckedDelete(l_NavMesh);
                }
            }
        }
        return true;
    }
}

void CNavMeshManager::Reload()
{
    Destroy();
    Load(m_Filename);
}
