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

bool CScene::Load(const std::string& aFilename)
{
    mFilename = aFilename;
    return Load();
}

bool CScene::Load(bool update)
{
    bool lOk = true;

    CXMLDocument document;
    EXMLParseError error = document.LoadFile((mFilename).c_str());
    std::set< std::string > lLayerNamesFromXML;

    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lScene = document.FirstChildElement("scene");
        if (lScene)
        {
            for (tinyxml2::XMLElement *iLayer = lScene->FirstChildElement(); iLayer != nullptr; iLayer = iLayer->NextSiblingElement())
            {
                if (strcmp(iLayer->Name(), "layer") == 0)
                {
                    const std::string lLayerName = iLayer->GetAttribute<std::string>("name", "");
                    lLayerNamesFromXML.insert(lLayerName);

                    CLayer* lLayer = (*this)(lLayerName);

                    if (lLayer)//(&& lLayer->GetActive()
                    {
                        lLayer->Load(iLayer, update);
                    }
                    else
                    {
                        lLayer = new CLayer(lLayerName);
                        lLayer->SetActive(iLayer->GetAttribute<bool>("active", false));

                        //if (lLayer->GetActive()){
                        lLayer->Load(iLayer);
                        lLayer->SetParent(this);
                        //}

                        lOk &= Add(lLayer->GetName(), lLayer);
                    }
                }
            }

            if (update)
            {
                std::set<std::string> lMissingLayers;
                for (std::vector<CLayer *>::iterator it = m_ResourcesVector.begin(); it != m_ResourcesVector.end(); ++it)
                {
                    if (lLayerNamesFromXML.find((*it)->GetName()) == lLayerNamesFromXML.end())
                    {
                        assert(lMissingLayers.count((*it)->GetName()) == 0);
                        lMissingLayers.insert((*it)->GetName());
                    }
                }

                for (std::set<std::string>::iterator iMissingLayer = lMissingLayers.begin(); iMissingLayer != lMissingLayers.end(); ++iMissingLayer)
                {
                    (*this)(*iMissingLayer)->DeleteAllNodes();
                    Remove(*iMissingLayer);
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
        if (iLayer->second.m_Value->GetActive())
            lOk &= iLayer->second.m_Value->Update(elapsedTime);
    }

    return lOk;
}

bool CScene::Render()
{
    bool lOk = true;

    for (TMapResources::iterator iLayer = m_ResourcesMap.begin(); iLayer != m_ResourcesMap.end(); ++iLayer)
    {
        if (iLayer->second.m_Value->GetActive())
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

CLayer* CScene::GetLayer(std::string aName)
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
        for (std::vector<CLayer*>::iterator iLayer = m_ResourcesVector.begin(); iLayer != m_ResourcesVector.end(); ++iLayer)
        {
            if (ImGui::TreeNode((*iLayer)->GetName().c_str()))
            {
                (*iLayer)->DrawImgui();
                ImGui::TreePop();
            }
        }
    }
}
