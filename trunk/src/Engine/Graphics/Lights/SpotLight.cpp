#include "SpotLight.h"
#include "ImGui/imgui.h"
#include "XML\xml.h"
#include "Engine\Engine.h"
#include "Graphics\Buffers\ConstantBufferManager.h"

CSpotLight::CSpotLight(CXMLElement* aElement)
    : CLight(aElement)
    , m_Angle(mathUtils::Deg2Rad(aElement->GetAttribute<float>("angle", 45.0f)))
    , m_FallOff(mathUtils::Deg2Rad(aElement->GetAttribute<float>("fall_off", 45.0f)))
{
    m_LightType = CLight::eSpot;
}

CSpotLight::~CSpotLight() {}

void CSpotLight::SetShadowMap(CRenderManager &RM)
{
    if (m_pShadowMap == NULL)
        return;

    m_ViewShadowMap.SetIdentity();
    GetForward();

    m_ViewShadowMap.SetFromLookAt(m_Position, m_Position + GetForward(), v3fY);
    unsigned int l_ShadowMapWidth = m_pShadowMap->GetSize().x;
    unsigned int l_ShadowMapHeight = m_pShadowMap->GetSize().y;
    m_ProjectionShadowMap.SetIdentity();
    m_ProjectionShadowMap.SetFromPerspective(m_FallOff, l_ShadowMapWidth / (float)l_ShadowMapHeight, 0.1f, m_RangeAttenuation.y);

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

void CSpotLight::DrawImgui()
{
    ImGui::SliderFloat("XPosition", (float*)&m_Position.x, mOriginalUnmodifiedPosition.x - 1.0f, mOriginalUnmodifiedPosition.x + 1.0f);
    ImGui::SliderFloat("YPosition", (float*)&m_Position.y, mOriginalUnmodifiedPosition.y - 1.0f, mOriginalUnmodifiedPosition.y + 1.0f);
    ImGui::SliderFloat("ZPosition", (float*)&m_Position.z, mOriginalUnmodifiedPosition.z - 1.0f, mOriginalUnmodifiedPosition.z + 1.0f);

    ImGui::SliderFloat("Fall Off Angle", &m_FallOff, -3.1416f, 3.1416f);
    ImGui::SliderFloat("Angle", &m_Angle, -3.1416f, 3.1416f);
    ImGui::SliderFloat("Intensity", &m_Intensity, 0.0f, 2.5f);

    float lYawTmp = mathUtils::Rad2Deg(m_Yaw);
    float lPitchTmp = mathUtils::Rad2Deg(m_Pitch);
    float lRollTmp = mathUtils::Rad2Deg(m_Roll);

    ImGui::SliderFloat("Yaw", (float*)&lYawTmp, -180.0f, 180.0f);
    ImGui::SliderFloat("Pitch", (float*)&lPitchTmp, -180.0f, 180.0f);
    ImGui::SliderFloat("Roll", (float*)&lRollTmp, -180.0f, 180.0f);

    m_Yaw = mathUtils::Deg2Rad(lYawTmp);
    m_Pitch = mathUtils::Deg2Rad(lPitchTmp);
    m_Roll = mathUtils::Deg2Rad(lRollTmp);

}

void CSpotLight::Initialize(CXMLElement* aElement)
{
    m_Angle = mathUtils::Deg2Rad(aElement->GetAttribute<float>("angle", 45.0f));
    m_FallOff = mathUtils::Deg2Rad(aElement->GetAttribute<float>("fall_off", 45.0f));
    CLight::Initialize(aElement);
}

