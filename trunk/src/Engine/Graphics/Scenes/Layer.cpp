#include "Layer.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "SceneMesh.h"
#include "SceneBasicPrimitive.h"
#include "Render/RenderManager.h"
#include "Engine/Engine.h"
#include "Graphics/Animation/SceneAnimatedModel.h"
#include "Graphics/Animation/AnimatedModelManager.h"
#include "Graphics/Lights/LightManager.h"
#include "SceneSkybox.h"

CLayer::CLayer(const std::string& aName) :
    CName(aName),
    m_Active(false)
{}

CLayer::~CLayer()
{
    CTemplatedMapVector<CSceneNode>::Destroy();
}

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
            lNode->Settype(1); //SceneAnimatedModel
        }
        else if (strcmp(iSceneMesh->Name(), "scene_basic_primitive") == 0)
        {
            lNode = new  CSceneBasicPrimitive(iSceneMesh);
            lNode->Settype(2); //SceneBasicPrimitive
        }
        else if (strcmp(iSceneMesh->Name(), "scene_light") == 0)
        {
            std::string l_lightName = iSceneMesh->GetAttribute<std::string>("name", "");

            CLight *l_light = nullptr;

            if (CEngine::GetInstance().GetLightManager().Exist(l_lightName)
                    && strcmp(iSceneMesh->FirstChildElement()->Name(), "transform") == 0)
            {

                l_light = CEngine::GetInstance().GetLightManager()(l_lightName);
                l_light->SetPosition(iSceneMesh->FirstChildElement()->GetAttribute<Vect3f>("position", l_light->GetPosition()));
                l_light->SetPosition(iSceneMesh->FirstChildElement()->GetAttribute<Vect3f>("forward", l_light->GetPrevPosition()));

                CEngine::GetInstance().GetLightManager().SetLightConstants(iSceneMesh->GetAttribute<int>("id_light", 0), l_light);
            }


            if (l_light != nullptr)//VER
            {
                lNode = new CSceneNode(iSceneMesh);
                lNode->Settype(3); //SceneLight
            }
        }
        if (lNode)
            lOk &= Add(lNode->GetName(), lNode);
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

    //for (std::vector<CSceneNode*>::iterator iSceneNode = m_ResourcesVector.begin(); iSceneNode != m_ResourcesVector.end(); ++iSceneNode)
    // #TODO: potencialmente ineficiente: estamos iterando un mapa en método render.
    //Set Light constants debe setearse en cada frame?? Qué pasa si desactivamos luces?
    for (TMapResources::iterator iSceneNode = m_ResourcesMap.begin(); iSceneNode != m_ResourcesMap.end(); ++iSceneNode)
    {
        CLight *lLight = CEngine::GetInstance().GetLightManager()(iSceneNode->second.m_Value->GetName());
        if (lLight != nullptr)
        {
            CEngine::GetInstance().GetLightManager().SetLightConstants(iSceneNode->second.m_Id, lLight);
        }

        lOk &= iSceneNode->second.m_Value->Render(lRenderManager);
    }

    return lOk;
}

std::vector<CSceneNode*> CLayer::GetNodes()
{
    return m_ResourcesVector;
}

void CLayer::DrawImgui()
{
    if (ImGui::CollapsingHeader(m_Name.c_str()))
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
                if (lSceneNode->Gettype() == 0) //Mesh
                    ((CSceneMesh *)lSceneNode)->DrawImgui();
                else if (lSceneNode->Gettype() == 1) // "scene_animated_model"
                {
                    ((CSceneAnimatedModel *)lSceneNode)->DrawImgui();
                    CAnimatedCoreModel *lAnimatedCoreModel = CEngine::GetInstance().GetAnimatedModelManager()(lSceneNode->GetName());
                    if (lAnimatedCoreModel != nullptr)
                        lAnimatedCoreModel->DrawImgui();
                }
                else if (lSceneNode->Gettype() == 2) // "scene_basic_primitive"
                    ((CSceneBasicPrimitive *)lSceneNode)->DrawImgui();
                else if (lSceneNode->Gettype() == 3) //"scene_light"
                {
                    CLight *lLight = CEngine::GetInstance().GetLightManager()(lSceneNode->GetName());
                    if (lLight != nullptr)
                        lLight->DrawImgui();
                }
                ImGui::PopID();
            }
            ImGui::PopItemWidth();
            ImGui::EndChild();
        }
    }
}
