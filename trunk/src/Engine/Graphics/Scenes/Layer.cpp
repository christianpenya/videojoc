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
#include "Graphics/Buffers/ConstantBufferManager.h"
#include "Graphics/Particles/ParticleManager.h"
#include "Graphics/Particles/ParticleSystemType.h"
#include "Graphics/IA/EnemiesManager.h"
#include "Graphics/IA/NavMesh.h"
#include "Graphics/IA/NavMeshManager.h"

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
    CEnemiesManager &lEnemiesManager = CEngine::GetInstance().GetEnemiesManager();
    CNavMeshManager &l_NavMeshManager = CEngine::GetInstance().GetNavMeshManager();

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
            lNode->SetNodeType(CSceneNode::eAnimatedModel);
        }
        else if (strcmp(iSceneMesh->Name(), "scene_basic_primitive") == 0)
        {
            lNode = new  CSceneBasicPrimitive(iSceneMesh);
            lNode->SetNodeType(CSceneNode::eBasicPrimitive);
        }
        else if (strcmp(iSceneMesh->Name(), "scene_light") == 0)
        {
            std::string l_lightName = iSceneMesh->GetAttribute<std::string>("name", "");
            CLight *l_light = nullptr;
            CConstantBufferManager& lConstanBufferManager = CEngine::GetInstance().GetConstantBufferManager();
            lConstanBufferManager.mLightsDesc.m_LightEnabled[0] = 0;
            lConstanBufferManager.mLightsDesc.m_LightEnabled[1] = 0;
            lConstanBufferManager.mLightsDesc.m_LightEnabled[2] = 0;
            lConstanBufferManager.mLightsDesc.m_LightEnabled[3] = 0;
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
        else if (strcmp(iSceneMesh->Name(), "scene_navmesh") == 0)
        {
            lNode = l_NavMeshManager(iSceneMesh->GetAttribute<std::string>("name", ""));
            lNode->SetNodeType(CSceneNode::eNavMesh);
        }
        else if (strcmp(iSceneMesh->Name(), "scene_enemies") == 0)
        {
            std::string l_EnemyName = iSceneMesh->GetAttribute<std::string>("name", "");
            CEnemy *l_enemy = nullptr;

            if (lEnemiesManager.Exist(l_EnemyName))
            {
                lNode = lEnemiesManager(l_EnemyName);
                lNode->SetNodeType(CSceneNode::eEnemy);
            }
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
    for (std::vector<CSceneNode*>::iterator iSceneNode = m_ResourcesVector.begin(); iSceneNode != m_ResourcesVector.end(); ++iSceneNode)
    {
        lOk &= (*iSceneNode)->Update(elapsedTime);
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
    std::set<std::string> lMissingNodes;

    for (TVectorResources::iterator iSceneNode = m_ResourcesVector.begin(); iSceneNode != m_ResourcesVector.end(); ++iSceneNode)
    {
        if ((*iSceneNode)->GetNodeType() == CSceneNode::eLight)
        {
            if (!lLM.Exist((*iSceneNode)->GetName()))
            {
                lMissingNodes.insert((*iSceneNode)->GetName());
            }
        }
    }

    for (std::set<std::string>::iterator iMissingNode = lMissingNodes.begin(); iMissingNode != lMissingNodes.end(); ++iMissingNode)
    {
        Remove(*iMissingNode);
    }

    return false;
}

std::vector<CSceneNode*> CLayer::GetNodes()
{
    return m_ResourcesVector;
}

void CLayer::DrawImgui()
{
    ImGui::Checkbox("Active", &m_Active);
    if (m_Active)
    {
//        ImGui::BeginChild("#layer", ImVec2(800, 400), true, ImGuiWindowFlags_AlwaysAutoResize);
//       ImGui::PushItemWidth(-130);

        for (std::vector<CSceneNode*>::iterator iSceneNode = m_ResourcesVector.begin(); iSceneNode != m_ResourcesVector.end(); ++iSceneNode)
        {
            ImGui::PushID((*iSceneNode)->GetName().c_str());

            switch ((*iSceneNode)->GetNodeType())
            {

            case CSceneMesh::eMesh:
            {
                (*iSceneNode)->DrawImgui();
            }
            break;

            case CSceneNode::eAnimatedModel:
            {
                // TODO animated core models deben formar parte de la escena
                ((CSceneAnimatedModel *)(*iSceneNode))->DrawImgui();
                CAnimatedCoreModel *lAnimatedCoreModel = CEngine::GetInstance().GetAnimatedModelManager()((*iSceneNode)->GetName());

                if (lAnimatedCoreModel != nullptr)
                    lAnimatedCoreModel->DrawImgui();
            }
            break;

            case CSceneAnimatedModel::eBasicPrimitive:
            {
                ((CSceneBasicPrimitive *)(*iSceneNode))->DrawImgui();
            }
            break;

            case CLight::eLight:
            {
                CLight *lLight = CEngine::GetInstance().GetLightManager()((*iSceneNode)->GetName());
                if (lLight != nullptr)
                    lLight->DrawImgui();
            }
            break;

            case CSceneNode::eParticle:
            {
                CParticleSystemType *lParticle = CEngine::GetInstance().GetParticleManager()((*iSceneNode)->GetName());
                if (lParticle != nullptr)
                    lParticle->DrawImgui();
            }
            break;
            case CSceneNode::eNavMesh:
            {
                CNavMesh *lNavMesh = CEngine::GetInstance().GetNavMeshManager()((*iSceneNode)->GetName());
                if (lNavMesh != nullptr)
                    lNavMesh->DrawImgui();
            }

            break;
            default:
            {
                LOG_WARNING_APPLICATION("Unknown Scene Node Type");
            }
            break;
            }

            ImGui::PopID();
        }

        /*        ImGui::PopItemWidth();
                ImGui::EndChild();*/
    }
}

CSceneNode* CLayer::GetSceneNode(std::string aName)
{
    return  m_ResourcesMap.find(aName)->second.m_Value;
}
