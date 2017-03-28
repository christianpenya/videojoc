#include "Texture.h"
#include "Utils\StringUtils.h"
#include "DirectXTex\DirectXTex.h"
#include "DirectXTex\dds.h"
#include "Render\RenderManager.h"
#include "Engine\Engine.h"
#include "Utils\CheckedDelete.h"
#include "Utils/Logger.h"

CTexture::CTexture(const std::string&  aName) :
    CName(aName),
    m_pTexture(nullptr),
    m_SamplerState(nullptr)
{}

CTexture::~CTexture() {}

bool CTexture::Load()
{
    // Convert the string filename to wide string
    std::wstring lTextureFileNameWString = std::wstring(m_Name.begin(), m_Name.end());
    std::string lExtension = base::utils::GetFileNameExtension(m_Name);

    HRESULT lHR = E_FAIL;
    lHR = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

    DirectX::TexMetadata lTexMetadata;
    DirectX::ScratchImage *lScratchImage = new DirectX::ScratchImage();

    if (lExtension == "dds")
        lHR = DirectX::LoadFromDDSFile(lTextureFileNameWString.c_str(), DirectX::DDS_FLAGS_NONE, &lTexMetadata, *lScratchImage);
    else if (lExtension == "tga")
        lHR = DirectX::LoadFromTGAFile(lTextureFileNameWString.c_str(), &lTexMetadata, *lScratchImage);
    else
        lHR = DirectX::LoadFromWICFile(lTextureFileNameWString.c_str(), DirectX::WIC_FLAGS_NONE, &lTexMetadata, *lScratchImage);

    if (SUCCEEDED(lHR))
    {
        CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();
        ID3D11Device *lDevice = lRenderManager.GetDevice();
        ID3D11DeviceContext *lDeviceContext = lRenderManager.GetDeviceContext();
        ID3D11Resource * lResource = nullptr;

        lHR = DirectX::CreateTexture(lDevice, lScratchImage->GetImages(), lScratchImage->GetImageCount(), lScratchImage->GetMetadata(), &lResource);

        if (SUCCEEDED(lHR))
        {
            lHR = DirectX::CreateShaderResourceView(lDevice, lScratchImage->GetImages(), lScratchImage->GetImageCount(), lScratchImage->GetMetadata(), &m_pTexture);

            if (SUCCEEDED(lHR))
            {
                ID3D11Texture2D * lTex2D = static_cast<ID3D11Texture2D *>(lResource);
                D3D11_TEXTURE2D_DESC desc;
                lTex2D->GetDesc(&desc);
                m_Size.x = desc.Width;
                m_Size.y = desc.Height;

                D3D11_SAMPLER_DESC l_SampDesc;
                ZeroMemory(&l_SampDesc, sizeof(l_SampDesc));
                l_SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                l_SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
                l_SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
                l_SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
                l_SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
                l_SampDesc.MinLOD = 0;
                l_SampDesc.MaxLOD = D3D11_FLOAT32_MAX;
                lHR = lRenderManager.GetDevice()->CreateSamplerState(&l_SampDesc, &m_SamplerState);
            }
        }
    }

    base::utils::CheckedDelete(lScratchImage);

    return SUCCEEDED(lHR);
}

void CTexture::Bind(uint32 aStageId, ID3D11DeviceContext*  aContext)
{
    aContext->PSSetSamplers	(aStageId, 1, &m_SamplerState);
    aContext->PSSetShaderResources(aStageId, 1, &m_pTexture);
}

void CTexture::Destroy()
{
}

bool CTexture::Reload()
{
    // #TODO
    return false;
}
