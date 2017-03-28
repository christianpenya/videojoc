#include "ApplyFilter.h"

CApplyFilter::CApplyFilter()
{

}

CApplyFilter::~CApplyFilter()
{

}

bool CApplyFilter::Load(const CXMLElement* aElement)
{
    bool lOk = CDrawQuad::Load(aElement);
    if (lOk)
    {
        mQuad = new CQuad();
        mQuad->Init();
    }
    return lOk;
}


void CApplyFilter::Execute(CRenderManager &lRM)
{

    for (size_t i = 0; i < m_DynamicTexturesMaterials.size(); ++i)
    {
        ID3D11RenderTargetView *l_RenderTargets[1];
        l_RenderTargets[0] = m_DynamicTexturesMaterials[i]->m_DynamicTexture->GetRenderTargetView();

        lRM.SetRenderTargets(1, &l_RenderTargets[0], nullptr);
        lRM.SetViewport(m_ViewportPosition, m_ViewportSize);
        m_DynamicTexturesMaterials[i]->m_Material->Apply();
        CTexture *l_Texture = (i == 0) ? m_StagedTextures[0]->m_Texture : m_DynamicTexturesMaterials[i-1]->m_DynamicTexture;
        l_Texture->Bind(0, lRM.GetDeviceContext());
        mQuad->Render();
        lRM.ResetViewport();
        lRM.UnsetRenderTargets();
    }

}