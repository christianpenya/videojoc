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
            // TODO Añadir condición dentro frustum + luz en escena activa
        {
            lLight->SetShadowMap(CEngine::GetInstance().GetRenderManager());
            lRM.Clear(true, true, CColor(lRM.m_BackgroundColor));
            CScene* lScene = lLight->GetParent()->GetParent();
            std::vector<std::string> lLayerNames = lLight->GetLayerNames();

            for (std::vector<std::string>::iterator it = lLayerNames.begin(); it != lLayerNames.end(); ++it)
            {
                CLayer* lLayer = lScene->GetLayerByName(*it);
                if (lLayer)
                    lLayer->Render();
            }
            lRM.ResetViewport();
            lRM.UnsetRenderTargets();
        }
    }
}
