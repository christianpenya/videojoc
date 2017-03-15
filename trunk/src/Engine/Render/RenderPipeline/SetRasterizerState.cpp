#include "SetRasterizerState.h"
#include "Engine\Engine.h"
#include "XML\XML.h"

CSetRasterizerState::CSetRasterizerState():
    m_RasterizerState(nullptr),
    m_CullMode(0),
    m_ClockWise(false),
    m_FillMode(0)
{}

CSetRasterizerState::~CSetRasterizerState() {}

bool CSetRasterizerState::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    if (lOk)
    {
        D3D11_CULL_MODE lCullMode;
        EnumString<D3D11_CULL_MODE>::ToEnum(lCullMode, aElement->GetAttribute<std::string>("cull_mode", "front"));
        m_CullMode = lCullMode;

        m_ClockWise = aElement->GetAttribute<bool>("clock_wise",0);

        D3D11_FILL_MODE lFillMode;
        EnumString<D3D11_FILL_MODE>::ToEnum(lFillMode, aElement->GetAttribute<std::string>("type", "solid"));
        m_FillMode = lFillMode;
    }
    return lOk;
}

bool CSetRasterizerState::CreateRasterizerState(CRenderManager& lRM)
{
    D3D11_RASTERIZER_DESC lRasterDesc;
    ZeroMemory(&lRasterDesc, sizeof(D3D11_RASTERIZER_DESC));
    lRasterDesc.FillMode = (D3D11_FILL_MODE)m_FillMode;
    lRasterDesc.AntialiasedLineEnable=false;
    lRasterDesc.CullMode = (D3D11_CULL_MODE)m_CullMode;
    lRasterDesc.DepthBias = 0;
    lRasterDesc.DepthBiasClamp = 0.0f;
    lRasterDesc.DepthClipEnable	= true;
    lRasterDesc.FrontCounterClockwise =	m_ClockWise;
    lRasterDesc.MultisampleEnable = false;
    lRasterDesc.ScissorEnable =	false;
    lRasterDesc.SlopeScaledDepthBias = 0.0f;
    // Create rasterizer state

    HRESULT	lHR = lRM.GetDevice()->CreateRasterizerState(&lRasterDesc,&m_RasterizerState);
    return SUCCEEDED(lHR);
}

void CSetRasterizerState::Execute(CRenderManager& lRM)
{
    CreateRasterizerState(lRM);
    lRM.GetDeviceContext()->RSSetState(m_RasterizerState);
}

void CSetRasterizerState::DrawIMGUI()
{
    if (ImGui::CollapsingHeader(m_Name.c_str()))
    {
        if (ImGui::CollapsingHeader("Cull Mode"))
        {
            ImGui::RadioButton("None", &m_CullMode, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Back", &m_CullMode, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Front", &m_CullMode, 1);
            ImGui::SameLine();
        }
        if (ImGui::CollapsingHeader("Fill Mode"))
        {
            ImGui::RadioButton("Solid", &m_FillMode, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Wireframe", &m_FillMode, 1);
            ImGui::SameLine();
        }
        ImGui::Checkbox("Clockwise", &m_ClockWise);
    }

}