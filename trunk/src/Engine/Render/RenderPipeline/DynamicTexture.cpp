#include "DynamicTexture.h"
#include "XML\XML.h"
#include "Utils\Logger.h"
#include "Engine\Engine.h"
#include "Render\RenderManager.h"

CDynamicTexture::CDynamicTexture(const CXMLElement *TreeNode)
    : CTexture(TreeNode->GetAttribute<std::string>("name", ""))
    , m_pRenderTargetTexture(nullptr)
    , m_pRenderTargetView(nullptr)
    , m_pDepthStencilBuffer(nullptr)
    , m_pDepthStencilView(nullptr)
    , m_CreateDepthStencilBuffer( TreeNode->GetAttribute<bool>("create_depth_stencil_buffer", false) )
{
    assert(TreeNode);

    if (!EnumString<TFormatType>::ToEnum(m_FormatType, TreeNode->GetAttribute<std::string>("format", "RGBA8_UNORM")))
        LOG_ERROR_APPLICATION("Invalid format type for dynamic texture");

    if (TreeNode->GetAttribute<bool>("texture_width_as_frame_buffer", false))
    {
        m_Size = CEngine::GetInstance().GetRenderManager().GetWindowSize();
    }
    else
    {
        m_Size = TreeNode->GetAttribute<Vect2u>("size", Vect2u(0,0));
    }
    Init();
}

CDynamicTexture::~CDynamicTexture()
{
    // TODO:Release the data
    /*m_pDepthStencilBuffer->Release();
    m_pDepthStencilView->Release();
    m_pRenderTargetTexture->Release();
    m_pRenderTargetView->Release();
    m_pTexture->Release();*/
}

void CDynamicTexture::Init()
{
    ID3D11Device *l_Device = CEngine::GetInstance().GetRenderManager().GetDevice();
    D3D11_TEXTURE2D_DESC l_textureDescription;

    ZeroMemory(&l_textureDescription, sizeof(D3D11_TEXTURE2D_DESC));
    l_textureDescription.Width = m_Size.x;
    l_textureDescription.Height = m_Size.y;
    l_textureDescription.MipLevels = 1;
    l_textureDescription.ArraySize = 1;
    l_textureDescription.Format = DXGI_FORMAT(m_FormatType);
    l_textureDescription.SampleDesc.Count = 1;
    l_textureDescription.Usage = D3D11_USAGE_DEFAULT;
    l_textureDescription.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    l_textureDescription.CPUAccessFlags = 0;
    l_textureDescription.MiscFlags = 0;
    HRESULT l_HR = l_Device->CreateTexture2D(&l_textureDescription, NULL, &m_pRenderTargetTexture);
    assert(!FAILED(l_HR));

    D3D11_RENDER_TARGET_VIEW_DESC l_RenderTargetViewDescription;
    l_RenderTargetViewDescription.Format = l_textureDescription.Format;
    l_RenderTargetViewDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    l_RenderTargetViewDescription.Texture2D.MipSlice = 0;
    l_HR = l_Device->CreateRenderTargetView(m_pRenderTargetTexture, &l_RenderTargetViewDescription, &m_pRenderTargetView);
    assert(!FAILED(l_HR));

    D3D11_SHADER_RESOURCE_VIEW_DESC l_ShaderResourceViewDescription;
    l_ShaderResourceViewDescription.Format = l_textureDescription.Format;
    l_ShaderResourceViewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    l_ShaderResourceViewDescription.Texture2D.MostDetailedMip = 0;
    l_ShaderResourceViewDescription.Texture2D.MipLevels = 1;

    l_HR = l_Device->CreateShaderResourceView(m_pRenderTargetTexture, &l_ShaderResourceViewDescription, &m_pTexture);
    assert(!FAILED(l_HR));

    if (m_CreateDepthStencilBuffer)
    {
        D3D11_TEXTURE2D_DESC l_DepthBufferDescription;
        ZeroMemory(&l_DepthBufferDescription, sizeof(D3D11_TEXTURE2D_DESC));

        l_textureDescription.Width = m_Size.x;
        l_textureDescription.Height = m_Size.y;
        l_DepthBufferDescription.MipLevels = 1;
        l_DepthBufferDescription.ArraySize = 1;
        l_DepthBufferDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        l_DepthBufferDescription.SampleDesc.Count = 1;
        l_DepthBufferDescription.SampleDesc.Quality = 0;
        l_DepthBufferDescription.Usage = D3D11_USAGE_DEFAULT;
        l_DepthBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        l_DepthBufferDescription.CPUAccessFlags = 0;
        l_DepthBufferDescription.MiscFlags = 0;
        l_HR = l_Device->CreateTexture2D(&l_DepthBufferDescription, NULL, &m_pDepthStencilBuffer);
        assert(!FAILED(l_HR));

        D3D11_DEPTH_STENCIL_VIEW_DESC l_DepthStencilViewDescription;
        ZeroMemory(&l_DepthStencilViewDescription, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

        l_DepthStencilViewDescription.Format = l_DepthBufferDescription.Format;

        l_DepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        l_DepthStencilViewDescription.Texture2D.MipSlice = 0;
        l_HR = l_Device->CreateDepthStencilView(m_pDepthStencilBuffer, &l_DepthStencilViewDescription, &m_pDepthStencilView);
        assert(!FAILED(l_HR));
    }
    else
    {
        m_pDepthStencilBuffer = nullptr;
    }

    CreateSamplerState();
}

bool CDynamicTexture::CreateSamplerState()
{
    ID3D11Device *l_Device = CEngine::GetInstance().GetRenderManager().GetDevice();
    D3D11_SAMPLER_DESC l_SampDesc;
    ZeroMemory(&l_SampDesc, sizeof(l_SampDesc));
    l_SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    l_SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    l_SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    l_SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    l_SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    l_SampDesc.MinLOD = 0;
    l_SampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    HRESULT l_HR = l_Device->CreateSamplerState(&l_SampDesc, &m_SamplerState);
    return SUCCEEDED(l_HR);
}
