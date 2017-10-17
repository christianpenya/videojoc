#include "EnableAlphaBlend.h"
#include "Engine/Engine.h"

CEnableAlphaBlend::CEnableAlphaBlend()
    : CRenderCmd()
{
    D3D11_BLEND_DESC l_AlphablendDesc;
    ZeroMemory(&l_AlphablendDesc, sizeof(D3D11_BLEND_DESC));
    l_AlphablendDesc.RenderTarget[0].BlendEnable = true;
    l_AlphablendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    l_AlphablendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    l_AlphablendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    l_AlphablendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    l_AlphablendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    //l_AlphablendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    //l_AlphablendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    l_AlphablendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    l_AlphablendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    CEngine::GetInstance().GetRenderManager().GetDevice()->CreateBlendState(&l_AlphablendDesc, &m_AlphaBlendState);
}

CEnableAlphaBlend::~CEnableAlphaBlend()
{
    //TODO release
}

bool CEnableAlphaBlend::Load(const CXMLElement* aElement)
{
    return CRenderCmd::Load(aElement);
}

void CEnableAlphaBlend::Execute(CRenderManager &RM)
{
    RM.GetDeviceContext()->OMSetBlendState(m_AlphaBlendState, NULL, 0xffffffff);
}

void CEnableAlphaBlend::DrawImgui() {}