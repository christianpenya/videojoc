#include "SetRasterizerState.h"
#include "Engine\Engine.h"
#include "XML\XML.h"


CSetRasterizerState::CSetRasterizerState()
{

}

CSetRasterizerState::~CSetRasterizerState()
{

}


bool CSetRasterizerState::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    if (lOk)
    {
        m_CullMode = aElement->GetAttribute<int>("cull_mode", 0);
        m_ClockWise = aElement->GetAttribute<bool>("clock_wise", false);

        D3D11_FILL_MODE lFillMode;
        EnumString<D3D11_FILL_MODE>::ToEnum(lFillMode, aElement->GetAttribute<std::string>("type", "solid"));
        m_FillMode = lFillMode;
        CreateRasterizerState();
    }
    return lOk;
}


bool CSetRasterizerState::CreateRasterizerState()
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
    HRESULT	lHR = CEngine::GetInstance().GetRenderManager().GetDevice()->CreateRasterizerState(&lRasterDesc,&m_RasterizerState);
    return SUCCEEDED(lHR);
}