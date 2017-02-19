#include "RenderImGUI.h"
#include "ImGUI\imgui.h"

#include "RenderSceneLayer.h"
#include "Utils\TemplatedMapVector.h"

#include "Lights\LightManager.h"
#include "Scenes\SceneManager.h"
#include "Scenes\Scene.h"
#include "Effects\ShaderManager.h"
#include "Effects\EffectManager.h"
#include "RenderPipeline\RenderPipeline.h"
#include "Cinematics\CinematicsManager.h"


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

    //FPS
    ImGui::Text("%.1f FPS", lEngine.m_FPS);
	if (ImGui::Button("PLAY"))
	{
		lEngine.GetCinematicManager().Play("Animation01");
		
	}
    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    // #TODO el contador de fps de imgui es bueno?

    // TECHNIQUES
    if (ImGui::CollapsingHeader("Cameras"))
    {
        // CAMERA SELECTION
        ImGui::RadioButton("Orbital", &lEngine.m_CameraSelector, 0);
        ImGui::SameLine();
        ImGui::RadioButton("FPS", &lEngine.m_CameraSelector, 1);
        ImGui::SameLine();
        ImGui::RadioButton("TPS", &lEngine.m_CameraSelector, 2);
    }

    // TECHNIQUES
    if (ImGui::CollapsingHeader("Reload"))
    {
        /*ImGui::Text("Reload -> ");
        ImGui::SameLine();
        */
        ImGui::PushID(RELOAD_SHADER_BUTTON_ID);
        ImGui::PushStyleColor(ImGuiCol_Button, GREEN);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, GREEN_HOVER);

        if (ImGui::Button("Shaders"))
        {
            lEngine.GetShaderManager().Reload();
            lEngine.GetEffectManager().Refresh();
        }
        ImGui::PopStyleColor(2);
        ImGui::PopID();

        ImGui::PushID(RELOAD_RENDERPIPELINE_BUTTON_ID);
        if (ImGui::Button("RenderPipeline"))
            lEngine.GetRenderPipeline().Reload();
        ImGui::PopID();

    }

    //Lights
    //lEngine.GetLightManager().DrawImgui();

    //SCENE ELEMENTS
    lEngine.GetSceneManager().DrawImgui();

    ImGui::End();
    ImGui::Render();
}