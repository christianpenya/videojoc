#include "Scene.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "XML/XML.h"
#include "Graphics/Lights/lightManager.h"
#include "Imgui/imgui.h"

CScene::CScene(const std::string& aName)
    : CName(aName),
      m_Active(false)
{}

CScene::~CScene()
{
    CTemplatedMapVector<CLayer>::Destroy();
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
    // #TODO: este render debe pintar todas las layers?
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

        if (lLayer->IsActive())
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

void CScene::DrawImGui()
{
    ImGui::Checkbox("Active", &m_Active);
    if (m_Active == true)
    {
        ImGui::BeginChild("#Layer", ImVec2(400, 400), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::PushItemWidth(-130);

        for (TMapResources::iterator iLayer = m_ResourcesMap.begin(); iLayer != m_ResourcesMap.end(); ++iLayer)
        {
            CLayer* llayer = iLayer->second.m_Value;
            ImGui::PushID(iLayer->second.m_Id);
            llayer->DrawImgui();
            ImGui::PopID();
        }
        ImGui::PopItemWidth();
        ImGui::EndChild();
    }
}