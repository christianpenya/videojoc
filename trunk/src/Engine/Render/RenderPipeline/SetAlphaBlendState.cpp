#include "SetAlphaBlendState.h"
#include "XML\XML.h"
#include "Engine\Engine.h"

CSetAlphaBlendState::CSetAlphaBlendState():
    m_AlphaBlendState(nullptr),
    m_enabled(false), m_SrcBlend(),
    m_DestBlend(),
    m_BlendOp(),
    m_SrcBlendAlpha(),
    m_DestBlendAlpha(),
    m_BlendOpAlpha(),
    m_render_target_write_mask()
{}

CSetAlphaBlendState::~CSetAlphaBlendState() {}

//Leerá el nodo:
/*<set_alpha_blend_state state="enabled" src_blend="SrcAlpha" dst_blend="InvSrcAlpha" blend_op="add"
src_blend_alpha = "blend_one" dst_blend_alpha = "blend_one" blend_op_alpha = "add"
render_target_write_mask = "color_write_enable_all" / > */

bool CSetAlphaBlendState::Load(const CXMLElement * aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    if (lOk && aElement->GetAttribute<std::string>("state", "enabled") == std::string("enabled"))
    {
        m_enabled=true;

        D3D11_BLEND l_SrcBlend;
        EnumString<D3D11_BLEND>::ToEnum(l_SrcBlend, aElement->GetAttribute<std::string>("src_blend", "SrcAlpha"));
        m_SrcBlend = l_SrcBlend;

        D3D11_BLEND l_DestBlend;
        EnumString<D3D11_BLEND>::ToEnum(l_DestBlend, aElement->GetAttribute<std::string>("dst_blend", "InvSrcAlpha"));
        m_DestBlend = l_DestBlend;

        D3D11_BLEND_OP l_BlendOp;
        EnumString<D3D11_BLEND_OP>::ToEnum(l_BlendOp, aElement->GetAttribute<std::string>("blend_op", "add"));
        m_BlendOp = l_BlendOp;

        D3D11_BLEND l_SrcBlendAlpha;
        EnumString<D3D11_BLEND>::ToEnum(l_SrcBlendAlpha, aElement->GetAttribute<std::string>("src_blend_alpha", "blend_one"));
        m_SrcBlendAlpha = l_SrcBlendAlpha;

        D3D11_BLEND l_DestBlendAlpha;
        EnumString<D3D11_BLEND>::ToEnum(l_DestBlendAlpha, aElement->GetAttribute<std::string>("dst_blend_alpha", "blend_one"));
        m_DestBlendAlpha = l_DestBlendAlpha;

        D3D11_BLEND_OP l_BlendOpAlpha;
        EnumString<D3D11_BLEND_OP>::ToEnum(l_BlendOpAlpha, aElement->GetAttribute<std::string>("blend_op_alpha", "add"));
        m_BlendOpAlpha = l_BlendOpAlpha;

        D3D11_COLOR_WRITE_ENABLE l_render_target_write_mask;
        EnumString<D3D11_COLOR_WRITE_ENABLE>::ToEnum(l_render_target_write_mask, aElement->GetAttribute<std::string>("render_target_write_mask", "all"));
        m_render_target_write_mask = l_render_target_write_mask;
    }
    return lOk;
}

bool CSetAlphaBlendState::CreateAlphaBlendState(CRenderManager& lRM)
{
    bool lOk = true;
    if (m_enabled)
    {
        D3D11_BLEND_DESC l_AlphablendDesc;
        ZeroMemory(&l_AlphablendDesc, sizeof(D3D11_BLEND_DESC));
        l_AlphablendDesc.RenderTarget[0].BlendEnable = true;
        l_AlphablendDesc.RenderTarget[0].SrcBlend = m_SrcBlend;
        l_AlphablendDesc.RenderTarget[0].DestBlend = m_DestBlend;
        l_AlphablendDesc.RenderTarget[0].BlendOp = m_BlendOp;
        l_AlphablendDesc.RenderTarget[0].SrcBlendAlpha = m_SrcBlendAlpha;
        l_AlphablendDesc.RenderTarget[0].DestBlendAlpha = m_DestBlendAlpha;
        l_AlphablendDesc.RenderTarget[0].BlendOpAlpha = m_BlendOpAlpha;
        l_AlphablendDesc.RenderTarget[0].RenderTargetWriteMask = m_render_target_write_mask;

        lOk = SUCCEEDED(lRM.GetDevice()->CreateBlendState(&l_AlphablendDesc, &m_AlphaBlendState));
    }
    return lOk;
}

void CSetAlphaBlendState::Execute(CRenderManager& lRM)
{
    CreateAlphaBlendState(lRM);
    lRM.GetDeviceContext()->OMSetBlendState(m_AlphaBlendState, NULL, 0xffffffff);
}
