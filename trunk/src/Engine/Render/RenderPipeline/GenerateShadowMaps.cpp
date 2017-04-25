#include "GenerateShadowMaps.h"
#include "Utils/Logger.h"
#include "Engine/Engine.h"
#include "Graphics/Lights/LightManager.h"
#include "Graphics/Lights/SpotLight.h"
#include "Graphics/Lights/DirectionalLight.h"
#include "Graphics/Scenes/Scene.h"
#include "Graphics/Scenes/SceneManager.h"

CGenerateShadowMaps::CGenerateShadowMaps() {}
CGenerateShadowMaps::~CGenerateShadowMaps() {}

bool CGenerateShadowMaps::Load(const CXMLElement* aElement)
{
    // Recorremos las escenas activas para encontrar todos aquellos nodos que sean luces que generan sombras
    std::vector<CScene*> lScenes = CEngine::GetInstance().GetSceneManager().GetResourcesVector();
    for (std::vector<CScene*>::iterator iScene = lScenes.begin(); iScene != lScenes.end(); ++iScene)
    {
        if ((*iScene)->GetActive())
        {
            std::vector<CLayer*> lLayers = (*iScene)->GetResourcesVector();
            for (std::vector<CLayer*>::iterator iLayer = lLayers.begin(); iLayer != lLayers.end(); ++iLayer)
            {
                if ((*iLayer)->GetActive())
                {
                    std::vector<CSceneNode*> lSceneNodes;
                    for (std::vector<CSceneNode*>::iterator iNode = lSceneNodes.begin(); iNode != lSceneNodes.end(); ++iNode)
                    {
                        if ((*iNode)->GetNodeType() == CSceneNode::eLight && (*iNode)->GetActive())
                        {
                            m_ShadowLights.push_back(dynamic_cast<CSpotLight*>(*iNode));
                        }
                    }
                }
            }
        }
    }

    LOG_INFO_APPLICATION("GenerateShadowMap Load");


    return true;
}

void CGenerateShadowMaps::Execute(CRenderManager& lRM)
{
    CLightManager& lLM = CEngine::GetInstance().GetLightManager();

    for (int i = 0; i < lLM.GetCount(); ++i)
    {
        CLight* lLight = lLM.GetLightByIdx(i);
        if (lLight->GetGenerateShadowMap() && lLight->GetActive())
        {
            lLight->SetShadowMap(CEngine::GetInstance().GetRenderManager());
            lRM.Clear(true, true, CColor(lRM.m_BackgroundColor));
            CScene* lScene = lLight->GetParent()->GetParent();
            std::vector<std::string> lLayerNames = lLight->GetLayerNames();

            for (std::vector<std::string>::iterator it = lLayerNames.begin(); it != lLayerNames.end(); ++it)
            {
                //CLayer* lLayer = lScene->GetLayerByName(*it);
                //if (lLayer)
                //lLayer->Render(lRM);
            }

            lRM.UnsetRenderTargets();
        }
    }
}
