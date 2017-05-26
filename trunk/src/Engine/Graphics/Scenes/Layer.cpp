#include "Layer.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "SceneMesh.h"
#include "SceneBasicPrimitive.h"
#include "Render/RenderManager.h"
#include "Engine/Engine.h"
#include "Graphics/Animation/SceneAnimatedModel.h"
#include "Graphics/Animation/AnimatedModelManager.h"
#include "Graphics/Lights/LightManager.h"
#include "Graphics/Particles/ParticleSystemInstance.h"
#include "Graphics/Particles/ParticleManager.h"
#include "Graphics/Particles/ParticleSystemType.h"

#ifdef _DEBUG
#include <chrono>
#include "Utils/Logger.h"
#include "Utils/MemLeaks/MemLeaks.h"
#endif

CLayer::CLayer(const std::string& aName) :
    CName(aName),
    CActive(false)
{}

CLayer::~CLayer()
{
    //CTemplatedMapVector<CSceneNode>::Destroy();
}

bool CLayer::Load(CXMLElement* aElement)
{
    bool lOk = true;
    CLightManager &lLM = CEngine::GetInstance().GetLightManager();

    for (tinyxml2::XMLElement *iSceneMesh = aElement->FirstChildElement(); iSceneMesh != nullptr; iSceneMesh = iSceneMesh->NextSiblingElement())
    {
        CSceneNode* lNode = nullptr;
        if (strcmp(iSceneMesh->Name(), "scene_mesh") == 0)
        {
            lNode = new CSceneMesh(iSceneMesh);
            lNode->SetNodeType(CSceneNode::eMesh);
        }
        else if (strcmp(iSceneMesh->Name(), "scene_animated_model") == 0)
        {
            std::string l_CoreName = iSceneMesh->GetAttribute<std::string>("core", "");
            CAnimatedCoreModel *l_AnimatedCoreModel = CEngine::GetInstance().GetAnimatedModelManager()(l_CoreName);
            if (l_AnimatedCoreModel != nullptr)
            {
                lNode = new CSceneAnimatedModel(*iSceneMesh);
                ((CSceneAnimatedModel *)lNode)->Initialize(l_AnimatedCoreModel);
            }
            lNode->SetNodeType(CSceneNode::eAnimatedModel); //SceneAnimatedModel
        }
        else if (strcmp(iSceneMesh->Name(), "scene_basic_primitive") == 0)
        {
            lNode = new  CSceneBasicPrimitive(iSceneMesh);
            lNode->SetNodeType(CSceneNode::eBasicPrimitive); //SceneBasicPrimitive
        }
        else if (strcmp(iSceneMesh->Name(), "scene_light") == 0)
        {
            std::string l_lightName = iSceneMesh->GetAttribute<std::string>("name", "");
            CLight *l_light = nullptr;

            if (lLM.Exist(l_lightName))
            {
                lNode = lLM(l_lightName);
                lLM.SetLightConstants(iSceneMesh->GetAttribute<int>("id_light", 0), lLM(l_lightName));
                lNode->SetNodeType(CSceneNode::eLight);
            }
        }
        else if (strcmp(iSceneMesh->Name(), "scene_particle") == 0)
        {
            lNode = new CParticleSystemInstance(iSceneMesh);
            lNode->SetNodeType(CSceneNode::eParticle);
        }

        if (lNode)
        {
            lNode->SetParent(this);
            lOk &= Add(lNode->GetName(), lNode);
        }
    }

    return lOk;
}

bool CLayer::Update(float elapsedTime)
{
    bool lOk = true;
    for (TMapResources::iterator iSceneNode = m_ResourcesMap.begin(); iSceneNode != m_ResourcesMap.end(); ++iSceneNode) // #TODO RECORRER VECTOR
    {
        lOk &= iSceneNode->second.m_Value->Update(elapsedTime);
    }
    return lOk;
}

bool CLayer::Render()
{
    bool lOk = true;
    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();

    for (TVectorResources::iterator iSceneNode = m_ResourcesVector.begin(); iSceneNode != m_ResourcesVector.end(); ++iSceneNode)
    {
        if ((*iSceneNode)->GetNodeType() != CSceneNode::eLight)
        {
            lOk &= (*iSceneNode)->Render(lRenderManager);
        }
    }

    return lOk;
}

bool CLayer::Refresh()
{
    CLightManager &lLM = CEngine::GetInstance().GetLightManager();

    for (TVectorResources::iterator iSceneNode = m_ResourcesVector.begin(); iSceneNode != m_ResourcesVector.end(); ++iSceneNode)
    {
        //if (iSceneNode->second.m_Value->GetNodeType() == CSceneNode::eLight)
        if ((*iSceneNode)->GetNodeType() == CSceneNode::eLight)
        {
            CSceneNode* bla = m_ResourcesMap.find((*iSceneNode)->GetName())->second.m_Value;
            bla = lLM((*iSceneNode)->GetName());
            bla->SetParent(this);
            LOG_INFO_APPLICATION("te mare");
            //CLight* tmp = Add //(*iSceneNode)->GetName();
            // iSceneNode->second.m_Value = lLM(iSceneNode->second.m_Value->GetName());
        }
    }

    return false;
}

std::vector<CSceneNode*> CLayer::GetNodes()
{
    return m_ResourcesVector;
}

void CLayer::DrawImgui()
{
    if (ImGui::CollapsingHeader(m_Name.c_str(), ImGuiWindowFlags_AlwaysVerticalScrollbar))
    {
        ImGui::Checkbox("Active", &m_Active);
        if (m_Active == true)
        {
            ImGui::BeginChild("#layer", ImVec2(400, 400), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
            ImGui::PushItemWidth(-130);

            for (TMapResources::iterator iLayerMapEntry = m_ResourcesMap.begin(); iLayerMapEntry != m_ResourcesMap.end(); ++iLayerMapEntry)
            {
                CSceneNode* lSceneNode = iLayerMapEntry->second.m_Value;
                ImGui::PushID(iLayerMapEntry->second.m_Id);
                if (lSceneNode->GetNodeType() == 0) //Mesh
                    ((CSceneMesh *)lSceneNode)->DrawImgui();
                else if (lSceneNode->GetNodeType() == 1) // "scene_animated_model"
                {
                    ((CSceneAnimatedModel *)lSceneNode)->DrawImgui();
                    CAnimatedCoreModel *lAnimatedCoreModel = CEngine::GetInstance().GetAnimatedModelManager()(lSceneNode->GetName());
                    if (lAnimatedCoreModel != nullptr)
                        lAnimatedCoreModel->DrawImgui();
                }
                else if (lSceneNode->GetNodeType() == 2) // "scene_basic_primitive"
                    ((CSceneBasicPrimitive *)lSceneNode)->DrawImgui();
                else if (lSceneNode->GetNodeType() == 3) //"scene_light"
                {
                    CLight *lLight = CEngine::GetInstance().GetLightManager()(lSceneNode->GetName());
                    if (lLight != nullptr)
                        lLight->DrawImgui();
                }
				else if (lSceneNode->GetNodeType() == 4) //"scene_particle"
				{
					CParticleSystemType *lParticle = CEngine::GetInstance().GetParticleManager()(lSceneNode->GetName());
					if (lParticle != nullptr)
						lParticle->DrawImgui();
				}
                ImGui::PopID();
            }
            ImGui::PopItemWidth();
            ImGui::EndChild();
        }
    }
}

