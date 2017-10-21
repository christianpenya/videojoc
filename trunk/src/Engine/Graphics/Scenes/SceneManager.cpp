#include "SceneManager.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "XML/XML.h"
#include "Imgui/imgui.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

#include <chrono>
#include "Utils/Logger.h"

CSceneManager::CSceneManager() {}
CSceneManager::~CSceneManager()
{
    CSceneManager::Destroy();
}

bool CSceneManager::Load(const std::string& aFilename)
{
    m_Filename = aFilename;
    return Load();
}

bool CSceneManager::Update(float elapsedTime)
{
    bool lOk = true;

    for (TMapResources::iterator iSceneMapEntry = m_ResourcesMap.begin(); iSceneMapEntry != m_ResourcesMap.end(); ++iSceneMapEntry)
    {
        CScene* lScene = iSceneMapEntry->second.m_Value;

        if (lScene->GetActive())
        {
            lOk &= lScene->Update(elapsedTime);
        }
    }
    return lOk;
}

bool CSceneManager::Render(const std::string& aLayer)
{
    bool lOk = true;

    for (TMapResources::iterator iSceneMapEntry = m_ResourcesMap.begin(); iSceneMapEntry != m_ResourcesMap.end(); ++iSceneMapEntry)
    {
        CScene* lScene = iSceneMapEntry->second.m_Value;

        if (lScene->GetActive())
        {
            lOk &= lScene->Render(aLayer);
        }
    }

    return lOk;
}

bool CSceneManager::ReloadCurrentScene()
{
    return GetCurrentScene()->Load(true);
}

void CSceneManager::Activate(const std::string& aScene, bool aBool)
{
    m_ResourcesMap.find(aScene)->second.m_Value->SetActive(aBool);
}

void CSceneManager::Destroy()
{
    base::utils::CTemplatedMapVector<CScene>::Destroy();
}

bool CSceneManager::Load()
{
    bool lOk = true;

    CXMLDocument document;
    EXMLParseError error = document.LoadFile((m_Filename).c_str());

    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lScenes = document.FirstChildElement("scenes");

        if (lScenes)
        {
            for (tinyxml2::XMLElement *iScene = lScenes->FirstChildElement(); iScene != nullptr; iScene = iScene->NextSiblingElement())
            {
                if (strcmp(iScene->Name(), "scene") == 0)
                {
                    bool lActive = iScene->GetAttribute<bool>("active", false);

                    if (lActive)
                    {
                        std::string lName = iScene->GetAttribute<std::string>("name", "");

                        CScene* lScene = new CScene(lName);
                        lScene->SetActive(iScene->GetAttribute<bool>("active", false));
                        std::string lFilename = iScene->GetAttribute<std::string>("folder", "") + lName;
                        std::string lExtension = ".xml";

                        if (lFilename.find(lExtension) == std::string::npos)
                        {
                            lFilename += lExtension;
                        }

                        lScene->Load(lFilename);
                        lOk &= Add(lScene->GetName(), lScene);
                    }
                }
            }
        }
    }

    return lOk;
}

std::vector<CScene*> CSceneManager::GetScenes()
{
    return m_ResourcesVector;
}

void CSceneManager::DrawImgui()
{
    for (std::vector<CScene*>::iterator iScene = m_ResourcesVector.begin(); iScene != m_ResourcesVector.end(); ++iScene)
    {
        if(ImGui::TreeNode((*iScene)->GetName().c_str()))
        {
            (*iScene)->DrawImGui();
            ImGui::TreePop();
        }

    }
}

CScene* CSceneManager::GetCurrentScene()
{
    for (std::vector<CScene*>::iterator iScene = m_ResourcesVector.begin(); iScene != m_ResourcesVector.end(); ++iScene)
    {
        if ((*iScene)->GetActive())
        {
            return (*iScene);
        }
    }
}