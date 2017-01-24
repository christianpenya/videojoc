#include "RenderImGUI.h"
#include "ImGUI\imgui.h"

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
    ImGui::Render();
}