#include "MaterialManager.h"
#include "Base/XML/XML.h"

void CMaterialManager::Load(const std::string & aLevelFilename, const std::string &aDefaultsFileName)
{
    m_LevelMaterialsFilename = aLevelFilename;
    LoadMaterialsFromFile(m_LevelMaterialsFilename, false);

    m_DefaultMaterialsFilename = aDefaultsFileName;
    LoadMaterialsFromFile(m_DefaultMaterialsFilename, true);
}

void CMaterialManager::Reload()
{
    LoadMaterialsFromFile(m_LevelMaterialsFilename, true);
    LoadMaterialsFromFile(m_DefaultMaterialsFilename, true);
}


bool CMaterialManager::LoadMaterialsFromFile(const std::string &Filename, bool UpdateFlag)
{

    bool out = false;

    CXMLDocument document;
    EXMLParseError error = document.LoadFile((m_materialsFolderPath + Filename).c_str());

    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lMaterials = document.FirstChildElement("materials");

        if (lMaterials)
        {
            for (tinyxml2::XMLElement *iMaterial = lMaterials->FirstChildElement(); iMaterial != nullptr; iMaterial = iMaterial->NextSiblingElement())
            {
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