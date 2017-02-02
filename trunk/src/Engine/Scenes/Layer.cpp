#include "Layer.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "SceneMesh.h"
#include "SceneBasicPrimitive.h"
#include "Render/RenderManager.h"
#include "Engine/Engine.h"
#include "Animation/SceneAnimatedModel.h"
#include "Animation/AnimatedModelManager.h"

CLayer::CLayer(const std::string& aName) : CName(aName) {}
CLayer::~CLayer() {}

bool CLayer::Load(CXMLElement* aElement)
{
    bool lOk = true;

    for (tinyxml2::XMLElement *iSceneMesh = aElement->FirstChildElement(); iSceneMesh != nullptr; iSceneMesh = iSceneMesh->NextSiblingElement())
    {
        CSceneNode* lNode = nullptr;
        if (strcmp(iSceneMesh->Name(), "scene_mesh") == 0)
        {
            lNode = new CSceneMesh(iSceneMesh);
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
        }
        else if (strcmp(iSceneMesh->Name(), "scene_basic_primitive") == 0)
        {
            lNode = new  CSceneBasicPrimitive(iSceneMesh);
        }
        if ( lNode )
            lOk &= Add(lNode->GetName(), lNode);
    }
    return lOk;
}

bool CLayer::Update(float elapsedTime)
{
    bool lOk = true;
    for (TMapResources::iterator iSceneNode = m_ResourcesMap.begin(); iSceneNode != m_ResourcesMap.end(); ++iSceneNode) //RECORRER VECTOR
    {
        lOk &= iSceneNode->second.m_Value->Update(elapsedTime);
    }
    return lOk;
}

bool CLayer::Render()
{
    bool lOk = true;
    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();

    for (TMapResources::iterator iSceneNode = m_ResourcesMap.begin(); iSceneNode != m_ResourcesMap.end(); ++iSceneNode)
    {
        lOk &= iSceneNode->second.m_Value->Render(lRenderManager);
    }

    return lOk;
}
