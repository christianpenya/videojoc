#include "MaterialManager.h"
#include "XML/XML.h"
#include "Imgui/imgui.h"
#include "Utils/Logger.h"

CMaterialManager::CMaterialManager() {}

CMaterialManager::~CMaterialManager()
{
    CTemplatedMap<CMaterial>::Destroy();
}

void CMaterialManager::Load(const std::string & aLevelFilename, const std::string &aDefaultsFileName)
{
    m_LevelMaterialsFilename = aLevelFilename;
    Load(m_LevelMaterialsFilename, false);

    m_DefaultMaterialsFilename = aDefaultsFileName;
    Load(m_DefaultMaterialsFilename, true);
}

void CMaterialManager::Reload()
{
    Load(m_LevelMaterialsFilename, true);
    Load(m_DefaultMaterialsFilename, true);
}

void CMaterialManager::Save()
{
    //TODO
}

bool CMaterialManager::Load(const std::string &Filename, bool UpdateFlag)
{
    bool out = false;

    CXMLDocument document;
    EXMLParseError error = document.LoadFile(Filename.c_str());

    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lMaterials = document.FirstChildElement("materials");

        if (lMaterials)
        {
            for (tinyxml2::XMLElement *iMaterial = lMaterials->FirstChildElement(); iMaterial != nullptr; iMaterial = iMaterial->NextSiblingElement())
            {
                //LOG_INFO_APPLICATION(iMaterial->GetAttribute<std::string>("name", "").c_str());

                if (strcmp(iMaterial->Name(), "material") == 0)
                {
                    CMaterial *lMaterial = new CMaterial(iMaterial);

                    if (!Add(lMaterial->GetName(), lMaterial))
                    {
                        if (UpdateFlag)
                        {
                            Update(lMaterial->GetName(), lMaterial);
                        }
                        else
                        {
                            base::utils::CheckedDelete(lMaterial);
                        }
                    }
                }
            }

            out = true;
        }
    }

    return out;
}

void CMaterialManager::DrawImgui()
{
    if (ImGui::CollapsingHeader("Material Manager"))
    {
        ImGui::BeginChild("#Scenes", ImVec2(400, 200), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::PushItemWidth(-130);

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

        ImGui::PopItemWidth();
        ImGui::EndChild();
    }
}