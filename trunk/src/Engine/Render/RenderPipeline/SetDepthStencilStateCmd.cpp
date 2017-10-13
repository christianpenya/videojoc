#include "SetDepthStencilStateCmd.h"
#include "Engine\Engine.h"
#include "XML\XML.h"

CSetDepthStencilStateCmd::CSetDepthStencilStateCmd():
    m_DepthStencilState(nullptr),
    m_EnableZTest(false),
    m_WriteZBuffer(false),
    m_EnableStencil(false),
    m_ComparisonFunc(0)
{}

CSetDepthStencilStateCmd::~CSetDepthStencilStateCmd() {}

//Leerá el siguiente nodo :
//<set_depth_stencil_state enable_z_test="false" write_z_buffer="false" enable_stencil = "false" / >
//FALTA COMPARISON_FUNC

bool CSetDepthStencilStateCmd::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    if (lOk)
    {
        m_EnableZTest = aElement->GetAttribute<bool>("enable_z_test", false);
        m_WriteZBuffer = aElement->GetAttribute<bool>("write_z_buffer", false);
        m_EnableStencil = aElement->GetAttribute<bool>("enable_stencil", false);
        m_ComparisonFunc = aElement->GetAttribute<int>("comparison_func", 4);
    }
    CRenderManager &l_RM = CEngine::GetInstance().GetRenderManager();
    return lOk && CreateDepthStencilState(l_RM);
}


bool CSetDepthStencilStateCmd::CreateDepthStencilState(CRenderManager& lRM)
{
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    // Depth test parameters
    dsDesc.DepthEnable = m_EnableZTest ? TRUE : FALSE;
    dsDesc.DepthWriteMask = m_WriteZBuffer ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    dsDesc.DepthFunc = D3D11_COMPARISON_FUNC(m_ComparisonFunc);
    // Stencil test parameters
    dsDesc.StencilEnable = m_EnableStencil ? TRUE : FALSE;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask	= 0xFF;
    // Stencil operations if pixel is front-facing
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    // Stencil operations if pixel is back-facing
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp= D3D11_STENCIL_OP_DECR;
    dsDesc.BackFace.StencilPassOp =	D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    // Create depth stencil state
    HRESULT lHR = lRM.GetDevice()->CreateDepthStencilState(&dsDesc,&m_DepthStencilState);
    return SUCCEEDED(lHR);
}

void CSetDepthStencilStateCmd::Execute(CRenderManager& lRM)
{
    lRM.GetDeviceContext()->OMSetDepthStencilState(m_DepthStencilState, 0);
}

void CSetDepthStencilStateCmd::DrawImgui()
{
    if (ImGui::TreeNode(m_Name.c_str()))
    {
        ImGui::PushID(m_Name.c_str());
        ImGui::Checkbox("Enable Z Test", &m_EnableZTest);
        ImGui::Checkbox("Write Z Buffer", &m_WriteZBuffer);
        ImGui::Checkbox("Enable Stencil", &m_EnableStencil);
        ImGui::SliderInt("Comparison Func", &m_ComparisonFunc, 1, 8);
        ImGui::PopID();
        ImGui::TreePop();
    }
}