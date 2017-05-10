#include "ApplyFilter.h"
#include "ImGUI/imgui.h"

CApplyFilter::CApplyFilter() : mQuad(nullptr)
{

}

CApplyFilter::~CApplyFilter()
{
    base::utils::CheckedDelete(mQuad);
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

        lRM.SetViewport(Vect2u(0, 0), m_DynamicTexturesMaterials[i]->m_DynamicTexture->GetSize());
        if (m_DynamicTexturesMaterials[i]->m_Material != nullptr)
        {
            m_DynamicTexturesMaterials[i]->m_Material->Apply();

            static bool show_app_auto_resize = true;
            ImGui::Begin("Menu Dynamic", &show_app_auto_resize, ImGuiWindowFlags_AlwaysAutoResize);
            m_DynamicTexturesMaterials[i]->m_Material->DrawImgui();
            ImGui::End();

        }
        ActivateTextures();
        CTexture *l_Texture = (i == 0) ? m_StagedTextures[0]->m_Texture : m_DynamicTexturesMaterials[i - 1]->m_DynamicTexture;
        l_Texture->Bind(0, lRM.GetDeviceContext());
        mQuad->Render();
        lRM.ResetViewport();
        lRM.UnsetRenderTargets();
    }
}
