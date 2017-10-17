#pragma once
#include "EnemiesManager.h"
#include "Enemy.h"
#include "Dron.h"
#include "Engine\engine.h"
#include "XML\xml.h"

CEnemiesManager::CEnemiesManager() {}
CEnemiesManager::~CEnemiesManager()
{
    CTemplatedMapVector<CEnemy>::Clear();
}

bool CEnemiesManager::Load(const std::string& aFilename)
{
    m_LevelEnemiesFilename = aFilename;
    return Load();
}

bool CEnemiesManager::Load()
{
    bool lOk = false;
    CXMLDocument document;
    EXMLParseError error = document.LoadFile((m_LevelEnemiesFilename).c_str());
    CEnemy::EEnemyType lEnemyType;

    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lEnemies = document.FirstChildElement("enemies");

        if (lEnemies)
        {
            for (tinyxml2::XMLElement *iEnemy = lEnemies->FirstChildElement(); iEnemy != nullptr; iEnemy = iEnemy->NextSiblingElement())
            {
                if (strcmp(iEnemy->Name(), "enemy") == 0)
                {
                    EnumString<CEnemy::EEnemyType>::ToEnum(lEnemyType, iEnemy->GetAttribute<std::string>("type", ""));

                    if (lEnemyType == CEnemy::eDron)
                    {
                        CDron* lEnemy = new CDron(iEnemy);
                        lOk = Add(lEnemy->GetName(), lEnemy);
                    }
                    else if (lEnemyType == CEnemy::eGuard)
                    {
                        /*CGuard* lEnemy = new CGuard(iEnemy);
                        lOk = Add(lEnemy->GetName(), lEnemy);*/
                    }
                }
            }
        }
    }

    return lOk;
}


bool CEnemiesManager::ReLoad()
{
    Clear();
    return Load();
}

void CEnemiesManager::DrawImgui()
{
    if (ImGui::TreeNode("Enemies"))
    {
        ImGui::ColorEditMode(ImGuiColorEditMode_RGB);
        for (TMapResources::iterator iEnemiesMapEntry = m_ResourcesMap.begin(); iEnemiesMapEntry != m_ResourcesMap.end(); ++iEnemiesMapEntry)
        {
            CEnemy* lEnemy = iEnemiesMapEntry->second.m_Value;
            ImGui::PushID(iEnemiesMapEntry->second.m_Id);
            lEnemy->DrawImgui();
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
}
