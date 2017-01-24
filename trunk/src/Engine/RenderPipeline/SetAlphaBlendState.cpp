#include "SetAlphaBlendState.h"
#include "XML\XML.h"
#include "Engine\Engine.h"

CSetAlphaBlendState::CSetAlphaBlendState()
{

}

CSetAlphaBlendState::~CSetAlphaBlendState()
{

}

//Leerá el nodo:
/*<set_alpha_blend_state state="enabled" src_blend="SrcAlpha" dst_blend="InvSrcAlpha" blend_op="add"
src_blend_alpha = ”blend_one” dst_blend_alpha = ”blend_one” blend_op_alpha = ”add”
render_target_write_mask = ”color_write_enable_all” / > */

bool CSetAlphaBlendState::Load(const CXMLElement * aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    if (lOk && aElement->GetAttribute<std::string>("state", "enabled") == std::string("enabled"))
    {
        D3D11_BLEND_DESC l_AlphablendDesc;
        ZeroMemory(&l_AlphablendDesc, sizeof(D3D11_BLEND_DESC));
        l_AlphablendDesc.RenderTarget[0].BlendEnable = true;
        // TODO Obtain the alpha blend states from the xml

        l_AlphablendDesc.RenderTarget[0].SrcBlend = aElement->GetAttribute<D3D11_BLEND>("src_blend", D3D11_BLEND_SRC_ALPHA);
        l_AlphablendDesc.RenderTarget[0].DestBlend = aElement->GetAttribute<D3D11_BLEND>("dst_blend", D3D11_BLEND_INV_SRC_ALPHA);
        l_AlphablendDesc.RenderTarget[0].BlendOp = aElement->GetAttribute<D3D11_BLEND_OP>("blend_op", D3D11_BLEND_OP_ADD);
        l_AlphablendDesc.RenderTarget[0].SrcBlendAlpha = aElement->GetAttribute<D3D11_BLEND>("src_blend_alpha", D3D11_BLEND_ONE);
        l_AlphablendDesc.RenderTarget[0].DestBlendAlpha = aElement->GetAttribute<D3D11_BLEND>("dst_blend_alpha", D3D11_BLEND_ONE);
        l_AlphablendDesc.RenderTarget[0].BlendOpAlpha = aElement->GetAttribute<D3D11_BLEND_OP>("blend_op_alpha", D3D11_BLEND_OP_ADD);
        l_AlphablendDesc.RenderTarget[0].RenderTargetWriteMask = aElement->GetAttribute<UINT8>("render_target_write_mask", D3D11_COLOR_WRITE_ENABLE_ALL);

        /*        l_AlphablendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
                l_AlphablendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
                l_AlphablendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
                l_AlphablendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
                l_AlphablendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
                l_AlphablendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                l_AlphablendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;*/
        CRenderManager& lRM = CEngine::GetInstance().GetRenderManager();
        lOk = SUCCEEDED(lRM.GetDevice()->CreateBlendState(&l_AlphablendDesc, &m_AlphaBlendState));
    }
    return lOk;
}

void CSetAlphaBlendState::Execute(CRenderManager& lRM)
{
    lRM.GetDeviceContext()->OMSetBlendState(m_AlphaBlendState, NULL, 0xffffffff);
}