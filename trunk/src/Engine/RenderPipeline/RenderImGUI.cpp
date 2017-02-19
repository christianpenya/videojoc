#include "RenderImGUI.h"
#include "ImGUI\imgui.h"

#include "RenderSceneLayer.h"
#include "Utils\TemplatedMapVector.h"

#include "Lights\LightManager.h"
#include "Scenes\SceneManager.h"
#include "Scenes\Scene.h"
#include "Effects\ShaderManager.h"
#include "Effects\EffectManager.h"
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
    //if (ImGui::CollapsingHeader("Reload"))
    ImGui::Text("Reload -> ");
    ImGui::SameLine();

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

    //SCENE ELEMENTS
    if (ImGui::CollapsingHeader("Scene Management"))
    {
        std::vector<CScene*> lScenes = lEngine.GetSceneManager().GetScenes();

        bool lSceneActive = false;
        bool lLayerActive = false;
        bool lNodeActive = false;
        std::string lSceneName;
        std::string lLayerName;
        std::string lNodeName;

        for (std::vector<CScene*>::iterator iScene = lScenes.begin(); iScene != lScenes.end(); ++iScene)
        {
            lSceneName = (*iScene)->GetName();
            lSceneActive = (*iScene)->IsActive();
            ImGui::Checkbox(lSceneName.c_str(), &lSceneActive);
            (*iScene)->SetActive(lSceneActive);

            if (lSceneActive)
            {
                std::vector<CLayer*> lLayers = (*iScene)->GetLayers();
                for (std::vector<CLayer*>::iterator iLayer = lLayers.begin(); iLayer != lLayers.end(); ++iLayer)
                {
                    lLayerActive = (*iLayer)->IsActive();
                    lLayerName = (*iLayer)->GetName();
                    ImGui::Text("");
                    ImGui::SameLine(25);
                    ImGui::Checkbox(lLayerName.c_str(), &lLayerActive);
                    (*iLayer)->SetActive(lLayerActive);

                    if (lLayerActive)
                    {
                        std::vector<CSceneNode*> lNodes = (*iLayer)->GetNodes();
                        for (std::vector<CSceneNode*>::iterator iNode = lNodes.begin(); iNode != lNodes.end(); ++iNode)
                        {
                            lNodeActive = (*iNode)->IsVisible();
                            lLayerName = (*iNode)->GetName();
                            ImGui::Text("");
                            ImGui::SameLine(50);
                            ImGui::Checkbox(lLayerName.c_str(), &lNodeActive);
                            (*iNode)->SetVisible(lNodeActive);
                        }
                    }
                }
            }
        }

        // CAMERA SELECTION
        ImGui::RadioButton("Orbital", &lEngine.m_CameraSelector, 0);
        ImGui::SameLine();
        ImGui::RadioButton("FPS", &lEngine.m_CameraSelector, 1);
        ImGui::SameLine();
        ImGui::RadioButton("TPS", &lEngine.m_CameraSelector, 2);
    }

    ImGui::End();
    ImGui::Render();
}