#include "XML\xml.h"
#include "DirectionalLight.h"
#include "Engine\Engine.h"
#include "Graphics\Buffers\ConstantBufferManager.h"

CDirectionalLight::CDirectionalLight(CXMLElement* aElement) :
    CLight(aElement)
    , m_OrthoShadowMapSize(aElement->GetAttribute<Vect2f>("ortho_shadow_map_size", Vect2f(30.0f, 40.0f)))
{
    m_LightType = CLight::eDirectional;
}

CDirectionalLight::~CDirectionalLight() {}

void CDirectionalLight::SetShadowMap(CRenderManager &RM)
{
    m_ViewShadowMap.SetIdentity();
    m_ViewShadowMap.SetFromLookAt(m_Position, m_Position + GetForward(), v3fY);
    unsigned int l_ShadowMapWidth = m_pShadowMap->GetSize().x;
    unsigned int l_ShadowMapHeight = m_pShadowMap->GetSize().y;
    // TODO near de la luz hardcodeado
    m_ProjectionShadowMap.SetFromOrtho(m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, 0.1f, m_RangeAttenuation.y);

    CConstantBufferManager& lCBM = CEngine::GetInstance().GetConstantBufferManager();
    RM.SetViewProjectionMatrix(m_ViewShadowMap, m_ProjectionShadowMap);
    lCBM.mFrameDesc.m_View = m_ViewShadowMap;
    lCBM.mFrameDesc.m_Projection = m_ProjectionShadowMap;
    lCBM.mFrameDesc.m_ViewProjection = RM.GetViewProjectionMatrix();
    lCBM.mFrameDesc.m_InverseView = lCBM.mFrameDesc.m_View.GetInverted();
    lCBM.mFrameDesc.m_InverseProjection = lCBM.mFrameDesc.m_Projection.GetInverted();
    lCBM.BindBuffer(RM.GetDeviceContext(), CConstantBufferManager::CB_Frame);

    ID3D11RenderTargetView *l_RenderTargetViews[1];
    l_RenderTargetViews[0] = m_pShadowMap->GetRenderTargetView();

    D3D11_VIEWPORT m_viewport;
    m_viewport.Width = (float)l_ShadowMapWidth;
    m_viewport.Height = (float)l_ShadowMapHeight;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;
    RM.GetDeviceContext()->RSSetViewports(1, &m_viewport);
    RM.SetRenderTargets(1, l_RenderTargetViews, m_pShadowMap->GetDepthStencilView());
}
