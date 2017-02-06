#include "RenderImGUI.h"
#include "ImGUI\imgui.h"
#include "Engine\engine.h"
#include "Lights\LightManager.h"
#include "Scenes\SceneManager.h"
#include "RenderSceneLayer.h"
#include "Utils\TemplatedMapVector.h"

CRenderImGUI::CRenderImGUI()
{

}

CRenderImGUI::~CRenderImGUI()
{

}

bool CRenderImGUI::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    return lOk;
}


void CRenderImGUI::Execute(CRenderManager& lRM)
{
    CEngine& lEngine = CEngine::GetInstance();

    static bool show_app_auto_resize = false;
    ImGui::Begin("Motor3D", &show_app_auto_resize, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Frame rate: ");
    ImGui::SameLine();
    ImGui::Text("FPS", 1 / lEngine.deltaTime, 0.0f, -1, 0);
    //ImGui::ShowTestWindow();

    ImGui::RadioButton("Orbital", &lEngine.cameraSelector, 0);
    ImGui::SameLine();
    ImGui::RadioButton("FPS", &lEngine.cameraSelector, 1);
    ImGui::SameLine();
    ImGui::RadioButton("TPS", &lEngine.cameraSelector, 2);

    lRM.DrawGrid(1, 1, 1, CColor(0, 0, 0, 1));
    /*switch (lEngine.cameraSelector)
    {
    case 0: //Orbital
        lRM.DrawSphere(1, CColor(1, 1, 0, 1));

        break;
    case 1: //FPS
        lRM.m_SphereOffset = Vect3f(0, 3, 0);
        lRM.DrawSphere(1, CColor(0, 1, 1, 1));

        break;
    case 2: //TPS
        lEngine.sphereRender(lRM);
        break;
    default:
        break;
    }

    if (CEngine::GetInstance().ExistSceneManager())
    {
    	CSceneManager& lSceneManager = CEngine::GetInstance().GetSceneManager();
        if (ImGui::CollapsingHeader("Scene"))
        {
            //              for (size_t i = 0; i < lSceneManager.GetCount(); ++i)
            //            {


            //          }
        }
    }
    */


    /*
    if (lLightManager.GetCount() > 0)
    {
    	CLightManager& lLightManager = CEngine::GetInstance().GetLightManager();
        if (ImGui::CollapsingHeader("Lights"))
        {
            /*			for (size_t i = 0; i < lLightManager.GetCount(); ++i)
            			{
            				//m_ResourcesVector[i]->Execute(lLightManager);

            				ImGui::Begin("Lights", &lLightManager, ImGuiWindowFlags_AlwaysAutoResize);
            				ImGui::End();
            			}
    	}
    }*/

    ImGui::End();
    ImGui::Render();
}