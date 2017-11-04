#include "GenerateShadowMaps.h"
#include "Utils/Logger.h"
#include "Engine/Engine.h"
#include "Graphics/Lights/LightManager.h"
#include "Graphics/Lights/SpotLight.h"
#include "Graphics/Lights/DirectionalLight.h"
#include "Graphics/Scenes/Scene.h"
#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Textures/TextureManager.h"

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
    CLight* lLight;

    for (int i = 0; i < lLM.GetCount(); ++i)
    {
        lLight = lLM.GetLightByIdx(i);
        if (lLight->GetGenerateShadowMap() && lLight->GetActive())
            // TODO Añadir condición dentro frustum + luz en escena activa
        {
            lLight->SetShadowMap(CEngine::GetInstance().GetRenderManager());
            lRM.Clear(true, true, CColor(0.0f, 0.0f, 0.0f, 0.0f));

            CScene* lScene = lLight->GetParent()->GetParent();
            std::vector<std::string> lLayerNames = lLight->GetLayerNames();
            lLM.SetLightConstants(0, lLight);

            for (std::vector<std::string>::iterator it = lLayerNames.begin(); it != lLayerNames.end(); ++it)
            {
                CLayer* lLayer = lScene->GetLayer(*it);
                if (lLayer)
                {
                    LOG_INFO_APPLICATION(("Rendering shadows for layer " + lLayer->GetName()).c_str());
                    lLayer->Render();
                }
            }

            lRM.UnsetRenderTargets();
        }
    }
}


void CGenerateShadowMaps::DrawImgui() {}