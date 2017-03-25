#include "PixelShader.h"
#include "Render\RenderManager.h"
#include "Engine\Engine.h"
#include "ShaderManager.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Utils/Logger.h"

CPixelShader::CPixelShader(const std::string& aShaderCode)
    : CShader(aShaderCode, EShaderStage::ePixelShader),
      m_pPixelShader(nullptr)
{}

CPixelShader::CPixelShader(const CXMLElement* aElement, const std::string& aPath)
    : CShader(aElement, aPath, EShaderStage::ePixelShader),
      m_pPixelShader(nullptr)
{}

CPixelShader::~CPixelShader() {}

bool CPixelShader::Load()
{
    bool lOk = CShader::Load();

    if (lOk)
    {
        CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();

        ID3D11Device *l_Device = lRenderManager.GetDevice();
        HRESULT lHR = l_Device->CreatePixelShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pPixelShader);
        lOk &= SUCCEEDED(lHR);
    }

    if (!lOk)
    {
        std::string lError = "Error cargando el shader -> " + m_Name;
        LOG_ERROR_APPLICATION(lError.c_str());
    }

    return lOk;
}

void CPixelShader::Bind(ID3D11DeviceContext* aContext)
{
    aContext->PSSetShader(m_pPixelShader, NULL, 0);
}

std::string CPixelShader::GetShaderModel()
{
    // Query the current feature level:
    D3D_FEATURE_LEVEL featureLevel = CEngine::GetInstance().GetRenderManager().GetDevice()->GetFeatureLevel();

    switch (featureLevel)
    {
    case D3D_FEATURE_LEVEL_11_1:
    case D3D_FEATURE_LEVEL_11_0:
    {
        return "ps_5_0";
    }
    break;
    case D3D_FEATURE_LEVEL_10_1:
    {
        return "ps_4_1";
    }
    break;
    case D3D_FEATURE_LEVEL_10_0:
    {
        return "ps_4_0";
    }
    break;
    case D3D_FEATURE_LEVEL_9_3:
    {
        return "ps_4_0_level_9_3";
    }
    break;
    case D3D_FEATURE_LEVEL_9_2:
    case D3D_FEATURE_LEVEL_9_1:
    {
        return "ps_4_0_level_9_1";
    }
    break;
    }
    return "";
}
