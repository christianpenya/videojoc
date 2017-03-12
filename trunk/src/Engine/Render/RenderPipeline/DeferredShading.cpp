#include "DeferredShading.h"
#include "Engine/Engine.h"
#include "XML\XML.h"


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

}

CDeferredShading::~CDeferredShading() {}

bool CDeferredShading::CreateBlendState(CRenderManager &lRM)
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
    lOk = SUCCEEDED(lRM.GetDevice()->CreateBlendState(&l_AlphablendDesc, &m_EnabledAlphaBlendState));
    return lOk;
}


bool CDeferredShading::Load(const CXMLElement* aElement)
{
    bool lOk = CDrawQuad::Load(aElement);

    for (tinyxml2::XMLElement const *iNTexture = aElement->FirstChildElement(); iNTexture != nullptr; iNTexture = iNTexture->NextSiblingElement())
    {
        if (strcmp(iNTexture->Name(), "texture") == 0)
        {
            CTexture* l_Texture = new CTexture(iNTexture->GetAttribute<std::string>("name", "AlbedoTexture"));
            CStagedTexture* l_StagedTexture = new CStagedTexture(iNTexture->GetAttribute<uint32>("stage_id", 1), l_Texture);
            m_StagedTextures.push_back(l_StagedTexture);
        }
    }
}

void CDeferredShading::Execute(CRenderManager &lRM)
{
    CreateBlendState(lRM);
    lRM.GetDeviceContext()->OMSetBlendState(m_EnabledAlphaBlendState, NULL, 0xffffffff);
}