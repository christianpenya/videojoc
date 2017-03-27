#include "CaptureFrameBufferTexture.h"
#include "XML\XML.h"
#include "Engine\Engine.h"
#include "Render\RenderManager.h"

CCaptureFrameBufferTexture::CCaptureFrameBufferTexture():
    CTexture(""),
    m_DataTexture(nullptr)
{}

CCaptureFrameBufferTexture::CCaptureFrameBufferTexture(const CXMLElement *TreeNode)
    : CTexture(TreeNode->GetAttribute<std::string>("name", "")),
      m_DataTexture(nullptr)
{
    if (TreeNode->GetAttribute<bool>("texture_width_as_frame_buffer", false))
    {
        mSize = CEngine::GetInstance().GetRenderManager().GetWindowSize();
    }
    else
    {
        mSize = TreeNode->GetAttribute<Vect2u>("size", Vect2u(0,0));
    }

    Init();
}

CCaptureFrameBufferTexture::~CCaptureFrameBufferTexture()
{
    // TODO:Release the data
}

void CCaptureFrameBufferTexture::Init()
{
    CRenderManager &l_RenderManager = CEngine::GetInstance().GetRenderManager();
    ID3D11Device *l_Device = l_RenderManager.GetDevice();

    ID3D11Texture2D *l_Buffer = NULL;

    l_RenderManager.GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&l_Buffer);

    D3D11_TEXTURE2D_DESC l_Texture2DDescription;
    l_Texture2DDescription.Width = mSize.x;
    l_Texture2DDescription.Height = mSize.y;
    l_Texture2DDescription.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    l_Texture2DDescription.ArraySize = 1;
    l_Texture2DDescription.MiscFlags = 0;
    l_Texture2DDescription.MipLevels = 1;
    l_Texture2DDescription.CPUAccessFlags = 0;
    l_Texture2DDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    l_Texture2DDescription.SampleDesc.Count = 1;
    l_Texture2DDescription.SampleDesc.Quality = 0;
    l_Texture2DDescription.Usage = D3D11_USAGE_DEFAULT;

    HRESULT l_HR = l_Device->CreateTexture2D(&l_Texture2DDescription, NULL, &m_DataTexture);
    if (FAILED(l_HR))
        return;

    D3D11_SHADER_RESOURCE_VIEW_DESC DescRV;
    ZeroMemory(&DescRV, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    DescRV.Format = l_Texture2DDescription.Format;
    DescRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    DescRV.Texture2D.MipLevels = l_Texture2DDescription.MipLevels;
    DescRV.Texture2D.MostDetailedMip = 0;

    l_HR = l_Device->CreateShaderResourceView(m_DataTexture, &DescRV, &m_pTexture);
    if (FAILED(l_HR))
        return;

    CreateSamplerState();
}

bool CCaptureFrameBufferTexture::CreateSamplerState()
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

bool CCaptureFrameBufferTexture::Capture(unsigned int StageId)
{
    CRenderManager &l_RenderManager = CEngine::GetInstance().GetRenderManager();
    ID3D11Texture2D *l_Surface = nullptr;
    HRESULT l_HR = l_RenderManager.GetSwapChain()->GetBuffer(StageId, __uuidof(ID3D11Texture2D), reinterpret_cast< void** >(&l_Surface));

    if (FAILED(l_HR) || l_Surface == nullptr || m_DataTexture == nullptr)
        return false;

    l_RenderManager.GetDeviceContext()->CopyResource(m_DataTexture, l_Surface);

    return true;
}
