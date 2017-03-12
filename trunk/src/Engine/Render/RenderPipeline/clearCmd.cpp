#include "ClearCmd.h"
#include "XML\XML.h"
#include "Utils\StringUtils.h"
#include "Render\RenderManager.h"
#include "ImGUI\imgui.h"

CClearCmd::CClearCmd(): m_RenderTarget(false), m_DepthStencil(false) {}
CClearCmd::~CClearCmd() {}

//Leerá el siguiente nodo :
//<clear name = "clear_cmd" render_target = "false" depth_stencil = "true" color = "0.25 0.25 0.25 0" / >

bool CClearCmd::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    if (lOk)
    {
        m_RenderTarget = aElement->GetAttribute<bool>("render_target", false);
        m_DepthStencil = aElement->GetAttribute<bool>("depth_stencil", true);
        m_Color = aElement->GetAttribute<CColor>("color", CColor(1, 1, 1, 1));
    }
    return lOk;
}

void CClearCmd::Execute(CRenderManager& lRM)
{
    lRM.Clear(m_RenderTarget, m_DepthStencil, m_Color);
}

void CClearCmd::DrawImGui()
{
    if (ImGui::CollapsingHeader(m_Name.c_str()))
    {
        ImGui::ColorEdit4("Color", (float*)&m_Color, true);
        ImGui::Checkbox("Render Target", &m_RenderTarget);
        ImGui::Checkbox("Depth Stencil", &m_DepthStencil);
    }
}