#include "RenderImGUI.h"
#include "ImGUI\imgui.h"

#include "RenderSceneLayer.h"
#include "Utils\TemplatedMapVector.h"

#include "Lights\LightManager.h"
#include "Scenes\SceneManager.h"
#include "Effects\Technique.h"
#include "Effects\TechniquePoolManager.h"

#include "RenderPipeline\RenderPipeline.h"
#include "RenderPipeline\RenderCmd.h"
#include "RenderPipeline\ApplyTechniquePool.h"

CRenderImGUI::CRenderImGUI() {}
CRenderImGUI::~CRenderImGUI() {}

bool CRenderImGUI::Load(const CXMLElement* aElement)
{
    return CRenderCmd::Load(aElement);
}

void CRenderImGUI::Execute(CRenderManager& lRM)
{
    CEngine& lEngine = CEngine::GetInstance();

    static bool show_app_auto_resize = false;
    ImGui::Begin("Bodegon3D", &show_app_auto_resize, ImGuiWindowFlags_AlwaysAutoResize);

    //ImGui::ShowTestWindow();

    //FPS
    ImGui::Text("%.1f FPS", lEngine.m_FPS);
    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // TECHNIQUES
    ImGui::Text("Techniques");
    std::vector<std::string> lPoolNames = lEngine.GetTechniquePoolManager().GetPoolNames();

    //for (std::vector<std::string>::const_iterator iTechniquePoolName = lPoolNames.begin();
    //	iTechniquePoolName != lPoolNames.end();
    //	++iTechniquePoolName)
    for (int i = 0; i < lPoolNames.size(); ++i)
    {
        ImGui::RadioButton(lPoolNames[i].c_str(), &lTechnique, i);
    }

    CRenderCmd* temare = lEngine.GetRenderPipeline()("apply_forward_technique");
    (dynamic_cast<CApplyTechniquePool*> (temare))->SetPoolName(lPoolNames[lTechnique]);

    lRM.DrawGrid(1, 1, 1, CColor(0, 0, 0, 1));

    ImGui::End();
    ImGui::Render();
}

// CAMERA SELECTION
/*ImGui::RadioButton("Orbital", (*mEngine).m_CameraSelector, 0);
ImGui::SameLine();
ImGui::RadioButton("FPS", &lEngine.m_CameraSelector, 1);
ImGui::SameLine();
ImGui::RadioButton("TPS", &lEngine.m_CameraSelector, 2);*/

// DEBUG OBJECTS

// LAYERS

// SCENES

// SHADERS