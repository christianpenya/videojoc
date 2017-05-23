#include "DeferredShading.h"
#include "Engine/Engine.h"
#include "XML\XML.h"
#include "Graphics/Lights/LightManager.h"


//Leera el nodo
//	<deferred_shading name="deffered_shading" material="DrawQuadDeferredShadingPerLightMaterial">
// <texture stage = "0" name = "AlbedoTexture" / >
// <texture stage = "1" name = "LightMapTexture" / >
// <texture stage = "2" name = "NormalMapTexture" / >
// <texture stage = "3" name = "DepthMapTexture" / >
// </ deferred_shading>


CDeferredShading::CDeferredShading()
    : CDrawQuad()
    , m_EnabledAlphaBlendState(nullptr)
{
    CreateBlendState();
}

CDeferredShading::~CDeferredShading() {}

bool CDeferredShading::CreateBlendState()
{
    bool lOk = true;
    D3D11_BLEND_DESC l_AlphablendDesc;
    ZeroMemory(&l_AlphablendDesc, sizeof(D3D11_BLEND_DESC));
    l_AlphablendDesc.RenderTarget[0].BlendEnable = true;
    l_AlphablendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    l_AlphablendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    l_AlphablendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    l_AlphablendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    l_AlphablendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    l_AlphablendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    l_AlphablendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    lOk = SUCCEEDED(CEngine::GetInstance().GetRenderManager().GetDevice()->CreateBlendState(&l_AlphablendDesc, &m_EnabledAlphaBlendState));
    return lOk;
}


bool CDeferredShading::Load(const CXMLElement* aElement)
{
    return CDrawQuad::Load(aElement);
}

void CDeferredShading::Execute(CRenderManager &lRM)
{
    //CreateBlendState(lRM);
    lRM.GetDeviceContext()->OMSetBlendState(m_EnabledAlphaBlendState, NULL, 0xffffffff);
    CLightManager* l_LM = &CEngine::GetInstance().GetLightManager();

    for (int i = 0; i < l_LM->GetCount(); i++)
    {
        l_LM->SetLightConstants(i, l_LM->GetLightByIdx(i));
        CDrawQuad::Execute(lRM);

    }
    lRM.GetDeviceContext()->OMSetBlendState(NULL, NULL, 0xffffffff);


}