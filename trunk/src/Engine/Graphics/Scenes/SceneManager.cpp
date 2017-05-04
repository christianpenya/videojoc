#include "SceneManager.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "XML/XML.h"
#include "Imgui/imgui.h"

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

bool CSceneManager::Reload()
{
    base::utils::CTemplatedMapVector<CScene>::Clear();
    return Load();
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
    return lOk;
}

std::vector<CScene*> CSceneManager::GetScenes()
{
    return m_ResourcesVector;
}


void CSceneManager::DrawImgui()
{
    if (ImGui::CollapsingHeader("Scenes Manager"))
    {
        ImGui::BeginChild("#Scenes", ImVec2(400, 400), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::PushItemWidth(-130);
        for (TMapResources::iterator iSceneMapEntry = m_ResourcesMap.begin(); iSceneMapEntry != m_ResourcesMap.end(); ++iSceneMapEntry)
        {
            CScene* lScene = iSceneMapEntry->second.m_Value;
            ImGui::PushID(iSceneMapEntry->second.m_Id);
            if (ImGui::CollapsingHeader(lScene->GetName().c_str()))
                lScene->DrawImGui();
            ImGui::PopID();
        }
        ImGui::PopItemWidth();
        ImGui::EndChild();
    }
}