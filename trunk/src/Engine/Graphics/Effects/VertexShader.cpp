#include "VertexShader.h"
#include "Graphics/Mesh/VertexTypes.h"
#include "Render/RenderManager.h"
#include "Engine/Engine.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "Utils/Logger.h"

CVertexShader::CVertexShader(const std::string& aShaderCode, uint32 aVertexFlags) :
    CShader(aShaderCode, EShaderStage::eVertexShader),
    m_VertexFlags(17),
    m_pVertexShader(nullptr),
    m_pVertexLayout(nullptr)
{}

CVertexShader::CVertexShader(const CXMLElement* aElement, const std::string& aPath) :
    CShader(aElement, aPath, EShaderStage::eVertexShader),
    m_pVertexShader(nullptr),
    m_pVertexLayout(nullptr)
{
    m_VertexFlags = VertexTypes::GetFlagsFromString(aElement->GetAttribute<std::string>("vertex_type", ""));
}

CVertexShader::~CVertexShader() {}

bool CVertexShader::Load()
{
    bool lOk = CShader::Load();

    if (lOk)
    {
        CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();
        HRESULT lHR = lRenderManager.GetDevice()->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pVertexShader);
        lOk = SUCCEEDED(lHR);

        lOk &= m_VertexFlags != 0;
        lOk &= VertexTypes::CreateInputLayout(lRenderManager, m_VertexFlags, m_pBlob, &m_pVertexLayout);
    }

    return lOk;
}

void CVertexShader::Bind(ID3D11DeviceContext* aContext)
{
    // Configure how the CPU will send the vertexes to the GPU( COORDS | NORMALS | COLOR | UV ... )
    aContext->IASetInputLayout(m_pVertexLayout);
    // Bind the vertex shader and its uniform data across all the vertexes
    aContext->VSSetShader(m_pVertexShader, NULL, 0);
}

std::string CVertexShader::GetShaderModel()
{
    // Query the current feature level:
    D3D_FEATURE_LEVEL featureLevel = CEngine::GetInstance().GetRenderManager().GetDevice()->GetFeatureLevel();
    switch (featureLevel)
    {
    case D3D_FEATURE_LEVEL_11_1:
    case D3D_FEATURE_LEVEL_11_0:
    {
        return "vs_5_0";
    }
    break;
    case D3D_FEATURE_LEVEL_10_1:
    {
        return "vs_4_1";
    }
    break;
    case D3D_FEATURE_LEVEL_10_0:
    {
        return "vs_4_0";
    }
    break;
    case D3D_FEATURE_LEVEL_9_3:
    {
        return "vs_4_0_level_9_3";
    }
    break;
    case D3D_FEATURE_LEVEL_9_2:
    case D3D_FEATURE_LEVEL_9_1:
    {
        return "vs_4_0_level_9_1";
    }
    break;
    } // switch( featureLevel )
    return "";
}
