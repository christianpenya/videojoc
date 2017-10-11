#include "SetRasterizerState.h"
#include "Engine\Engine.h"
#include "XML\XML.h"
#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR)/sizeof(*_ARR)))

CSetRasterizerState::CSetRasterizerState():
    m_RasterizerState(nullptr),
    m_CullMode(D3D11_CULL_FRONT),
    m_ClockWise(false),
    m_FillMode(D3D11_FILL_SOLID)
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
    lRasterDesc.FillMode = m_FillMode;
    lRasterDesc.AntialiasedLineEnable=false;
    lRasterDesc.CullMode = m_CullMode;
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

void CSetRasterizerState::DrawImgui()
{
    if (ImGui::TreeNode("SetRasterizerState"))
    {
        const char* items[] = { "none","back","front" };
        ImGui::Combo("Cull Mode", (int*)&m_CullMode, items, IM_ARRAYSIZE(items));
        const char* items2[] = { "solid", "wireframe"};
        ImGui::Combo("Fill Mode", (int*)&m_FillMode, items2, IM_ARRAYSIZE(items2));
        ImGui::Checkbox("Clockwise", &m_ClockWise);
        ImGui::TreePop();
    }

}
