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
        lRM.SetViewport(m_ViewportPosition, m_ViewportSize);
        ActivateTextures();
        m_DynamicTexturesMaterials[i]->m_Material->Apply();
//        m_StagedTextures[0]->m_Texture = m_DynamicTexturesMaterials[i]->m_Material->GetTexture(0);//como obtengo el indice
        mQuad->Render();
        lRM.ResetViewport();
    }

}