#include "MaterialManager.h"
#include "XML/XML.h"
#include "Imgui/imgui.h"
#include "Utils/Logger.h"
#include <cassert>

CMaterialManager::CMaterialManager() {}

CMaterialManager::~CMaterialManager()
{
    CTemplatedMap<CMaterial>::Destroy();
}

void CMaterialManager::Load(const std::string & aLevelFilename, const std::string &aDefaultsFileName)
{
    m_LevelMaterialsFilename = aLevelFilename;
    m_DefaultMaterialsFilename = aDefaultsFileName;

    std::set< std::string > lMaterialFilenames;
    lMaterialFilenames.insert(m_LevelMaterialsFilename);
    lMaterialFilenames.insert(m_DefaultMaterialsFilename);

    Load(lMaterialFilenames);
}


bool CMaterialManager::Load(bool update)
{
    bool out = true;

    std::set< std::string > lMaterialFilenames;
    lMaterialFilenames.insert(m_LevelMaterialsFilename);
    lMaterialFilenames.insert(m_DefaultMaterialsFilename);

    out &= Load(lMaterialFilenames, update);

    return out;
}

bool CMaterialManager::Load(std::set< std::string > &filenames, bool update)
{
    CXMLDocument document;
    std::set< std::string > lMaterialNamesFromXML;
    //assert(lMaterialNamesFromXML.count(lMaterialName) == 0);

    for (auto iFilename : filenames)
    {
        EXMLParseError error = document.LoadFile(iFilename.c_str());

        if (base::xml::SucceedLoad(error))
        {
            CXMLElement * lMaterials = document.FirstChildElement("materials");

            if (lMaterials)
            {
                for (tinyxml2::XMLElement *iMaterial = lMaterials->FirstChildElement(); iMaterial != nullptr; iMaterial = iMaterial->NextSiblingElement())
                {
                    LOG_INFO_APPLICATION(iMaterial->GetAttribute<std::string>("name", "").c_str());
                    if (strcmp(iMaterial->Name(), "material") == 0)
                    {
                        const std::string lMaterialName = iMaterial->GetAttribute<std::string>("name", "");
                        lMaterialNamesFromXML.insert(lMaterialName);

                        CMaterial* lMaterial = (*this)(lMaterialName);

                        if (lMaterial)
                        {
                            lMaterial->ClearParametersAndTextures();
                            lMaterial->Initialize(iMaterial);
                        }
                        else
                        {
                            lMaterial = new CMaterial(iMaterial);
                            Update(lMaterial->GetName(), lMaterial);
                        }
                    }
                }
            }
        }
    }

    if (update)
    {
        std::set<std::string> lMissingMaterials;
        for (std::map<std::string, CMaterial*>::iterator iMaterial = m_ResourcesMap.begin(); iMaterial != m_ResourcesMap.end(); ++iMaterial)
        {
            if ((*iMaterial).first != "" && iMaterial->second != NULL)
            {
                std::string temp = (*iMaterial).second->GetName();
                auto burru = lMaterialNamesFromXML.find(temp);
                if (lMaterialNamesFromXML.find(temp) == lMaterialNamesFromXML.end())
                {
                    assert(lMissingMaterials.count(temp) == 0);
                    lMissingMaterials.insert(temp);
                }
            }
        }

        for (std::set<std::string>::iterator iMissingMaterial = lMissingMaterials.begin(); iMissingMaterial != lMissingMaterials.end(); ++iMissingMaterial)
        {
            Remove(*iMissingMaterial);
        }
    }


    return true;
}

void CMaterialManager::DrawImgui()
{
    if (ImGui::TreeNode("Materiales"))
    {
        for (std::map<std::string, CMaterial*>::iterator iMaterial = m_ResourcesMap.begin(); iMaterial != m_ResourcesMap.end(); ++iMaterial)
        {
            LOG_INFO_APPLICATION(iMaterial->first.c_str());

            if (iMaterial->first != "" && iMaterial->second != NULL)
            {
                ImGui::PushID(iMaterial->second->GetName().c_str());
                iMaterial->second->DrawImgui();
                ImGui::PopID();
            }
        }

        ImGui::TreePop();
    }
}
