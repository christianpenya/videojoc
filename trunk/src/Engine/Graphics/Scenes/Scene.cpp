#include "Scene.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "XML/XML.h"
#include "Graphics/Lights/lightManager.h"
#include "Imgui/imgui.h"

#ifdef _DEBUG

#include "Utils/MemLeaks/MemLeaks.h"
#endif
#include <chrono>
#include "Utils/Logger.h"

CScene::CScene(const std::string& aName)
    : CName(aName),
      CActive(false)
{}

CScene::~CScene()
{
    CTemplatedMapVector<CLayer>::Destroy();
}

bool CScene::Refresh()
{
    for (TVectorResources::iterator iLayer = m_ResourcesVector.begin(); iLayer != m_ResourcesVector.end(); ++iLayer)
    {
        if ((*iLayer)->GetActive())
        {
            LOG_INFO_APPLICATION(("Refresh layer " + (*iLayer)->GetName() + std::to_string(clock())).c_str());
            (*iLayer)->Refresh();
        }
    }

    return true;
}

bool CScene::Load(const std::string& aFilename)
{
    bool lOk = true;
    CXMLDocument document;
    EXMLParseError error = document.LoadFile((aFilename).c_str());

    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lScene = document.FirstChildElement("scene");
        if (lScene)
        {
            for (tinyxml2::XMLElement *iLayer = lScene->FirstChildElement(); iLayer != nullptr; iLayer = iLayer->NextSiblingElement())
            {
                if (strcmp(iLayer->Name(), "layer") == 0)
                {
                    CLayer* lLayer = new CLayer(iLayer->GetAttribute<std::string>("name", ""));
                    lLayer->SetActive(iLayer->GetAttribute<bool>("active", false));
                    lLayer->Load(iLayer);
                    lLayer->SetParent(this);
                    lOk &= Add(lLayer->GetName(), lLayer);
                }
            }
        }
    }

    return lOk;
}

bool CScene::Update(float elapsedTime)
{
    bool lOk = true;

    for (TMapResources::iterator iLayer = m_ResourcesMap.begin(); iLayer != m_ResourcesMap.end(); ++iLayer)
    {
        lOk &= iLayer->second.m_Value->Update(elapsedTime);
    }

    return lOk;
}

bool CScene::Render()
{
    bool lOk = true;

    for (TMapResources::iterator iLayer = m_ResourcesMap.begin(); iLayer != m_ResourcesMap.end(); ++iLayer)
    {
        lOk &= iLayer->second.m_Value->Render();
    }

    return lOk;
}

bool CScene::Render(const std::string& aLayerName)
{
    bool lOk = true;

    if (Exist(aLayerName))
    {
        CLayer* lLayer = m_ResourcesMap.find(aLayerName)->second.m_Value;

        if (lLayer->GetActive())
        {
            lOk &= lLayer->Render();
        }
    }

    return lOk;
}

std::vector<CLayer*> CScene::GetLayers()
{
    return m_ResourcesVector;
}

CLayer* CScene::GetLayerByName(std::string aName)
{
    CLayer* lOut = nullptr;

    if (Exist(aName))
    {
        lOut = m_ResourcesMap[aName].m_Value;
    }

    return lOut;
}

void CScene::DrawImGui()
{
    ImGui::Checkbox("Active", &m_Active);
    if (m_Active)
    {
        ImGui::BeginChild("#Layer", ImVec2(400, 200), true);
        ImGui::PushItemWidth(-130);

        for (std::vector<CLayer*>::iterator iLayer = m_ResourcesVector.begin(); iLayer != m_ResourcesVector.end(); ++iLayer)
        {
            ImGui::PushID((*iLayer)->GetName().c_str());
            static bool show_app_auto_resize = true;

            if (ImGui::CollapsingHeader((*iLayer)->GetName().c_str()))
            {
                (*iLayer)->DrawImgui();
            }

            ImGui::PopID();
        }
        ImGui::PopItemWidth();
        ImGui::EndChild();
    }
}
