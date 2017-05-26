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

CRenderImGUI::CRenderImGUI() {}
CRenderImGUI::~CRenderImGUI() {}

bool CRenderImGUI::Load(const CXMLElement* aElement)
{
    return CRenderCmd::Load(aElement);
}

void CRenderImGUI::Execute(CRenderManager& lRM)
{
    CEngine& lEngine = CEngine::GetInstance();

    static bool show_app_auto_resize = true;
    ImGui::Begin("Menu", &show_app_auto_resize, ImGuiWindowFlags_AlwaysAutoResize);

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

    // RELOADS
    Reloads(lEngine);

    //SCENE ELEMENTS
    lEngine.GetSceneManager().DrawImgui();

    ImGui::End();
    ImGui::Render();
}

void CRenderImGUI::Reloads(CEngine& lEngine)
{
    // TECHNIQUES
    if (ImGui::CollapsingHeader("Reload"))
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
            lEngine.GetLightManager().Reload();
            lEngine.GetSceneManager().Refresh();
        }
        ImGui::PopStyleColor(2);
        ImGui::PopID();

        //RENDER PIPELINE
        ImGui::PushID(RELOAD_RENDERPIPELINE_BUTTON_ID);
        if (ImGui::Button("RenderPipeline"))
            lEngine.GetRenderPipeline().Reload();
        ImGui::PopID();
    }
}