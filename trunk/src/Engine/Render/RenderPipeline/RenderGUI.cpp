#include "RenderGUI.h"
#include "GUI/GUIManager.h"

#include "RenderSceneLayer.h"

#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Effects/ShaderManager.h"
#include "Graphics/Effects/EffectManager.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Graphics/Cinematics/CinematicsManager.h"
#include "Render/RenderPipeline/SetRasterizerState.h"

CRenderGUI::CRenderGUI() {}
CRenderGUI::~CRenderGUI() {}

bool CRenderGUI::Load(const CXMLElement* aElement)
{
    return CRenderCmd::Load(aElement);
}

void CRenderGUI::Execute(CRenderManager& lRM)
{
    CGUIManager& lGUIMan = CEngine::GetInstance().GetGUIManager();
    lGUIMan.Render(&CEngine::GetInstance().GetRenderManager());

}

void CRenderGUI::DrawImgui() {}