// si estas capturando mouse o keyboard
//ImGui::GetIO().WantCaptureMouse;

#include "RenderImGUI.h"
#include "ImGUI\imgui.h"

#include "RenderSceneLayer.h"

#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Effects/ShaderManager.h"
#include "Graphics/Effects/EffectManager.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Graphics/Cinematics/CinematicsManager.h"
#include "Render/RenderPipeline/SetRasterizerState.h"
#include "Graphics/Lights/LightManager.h"
#include "Graphics/Camera/CameraManager.h"

CRenderImGUI::CRenderImGUI() {}
CRenderImGUI::~CRenderImGUI() {}

bool CRenderImGUI::Load(const CXMLElement* aElement)
{
    return CRenderCmd::Load(aElement);
}

void CRenderImGUI::Execute(CRenderManager& lRM)
{
    CEngine& lEngine = CEngine::GetInstance();

    static bool show_cameras = false;

    if (show_cameras)
    {
        ImGui::Begin("Camera Selection", &show_cameras, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Separator();
        ImGui::RadioButton("TPS", &lEngine.GetCameraManager().m_CameraSelector, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Free", &lEngine.GetCameraManager().m_CameraSelector, 1);
        ImGui::End();
    }



    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Main"))
        {
            ImGui::MenuItem("Cameras", NULL, &show_cameras);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene Manager"))
        {
            lEngine.DrawImgui(0);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Sound Manager"))
        {
            lEngine.DrawImgui(1);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Reloads"))
        {
            Reloads(lEngine);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Shaders"))
        {
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

    }

    ImGui::Render();

    /*    //FPS
    	ImGui::Text("%.1f FPS", lEngine.m_FPS);
    	if (ImGui::Button("PLAY"))
    	{
    	lEngine.GetCinematicManager().Play("Animation01");
    	}
    	*/



}

void CRenderImGUI::Reloads(CEngine& lEngine)
{
    if (ImGui::TreeNode(m_Name.c_str()))
    {
        //SHADERS
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

        //ESCENA
        ImGui::PushID(RELOAD_SHADER_BUTTON_ID);
        ImGui::PushStyleColor(ImGuiCol_Button, GREEN);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, GREEN_HOVER);

        if (ImGui::Button("Scene"))
        {
            lEngine.GetSceneManager().Reload();
            //lEngine.GetEffectManager().Refresh();
        }

        ImGui::PopStyleColor(2);
        ImGui::PopID();

        //LUCES
        ImGui::PushID(RELOAD_LIGHTS_BUTTON_ID);
        ImGui::PushStyleColor(ImGuiCol_Button, GREEN);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, GREEN_HOVER);

        if (ImGui::Button("Luces"))
        {
            lEngine.GetLightManager().Load(true);
            //lEngine.GetSceneManager().Refresh();
        }
        ImGui::PopStyleColor(2);
        ImGui::PopID();

        //RENDER PIPELINE
        ImGui::PushID(RELOAD_RENDERPIPELINE_BUTTON_ID);
        if (ImGui::Button("RenderPipeline"))
            lEngine.GetRenderPipeline().Reload();
        ImGui::PopID();
        ImGui::TreePop();

    }
}
