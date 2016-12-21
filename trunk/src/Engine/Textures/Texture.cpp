#include "Texture.h"
#include "Utils\StringUtils.h"
#include "DirectX\DDSTextureLoader.h"
#include "DirectX\WICTextureLoader.h"
#include "Render\RenderManager.h"
#include "Engine\Engine.h"

bool CTexture::Load()
{
	ID3D11Resource *  lResource= nullptr;
	// Convert the string filename to wide string
	std::wstring lTextureFileNameWString = std::wstring(m_Name.begin(), m_Name.end());
	std::string lExtension = base::utils::GetFileNameExtension(m_Name);
	CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();

	HRESULT lHR = E_FAIL;
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
	if (lHR == E_FAIL)
		return false;

	lHR = E_FAIL;
	if (lExtension == "dds")
		lHR = DirectX::CreateDDSTextureFromFile(lRenderManager.GetDevice(), lRenderManager.GetDeviceContext(), lTextureFileNameWString.c_str(), &lResource, &m_pTexture);
	else
		lHR = DirectX::CreateWICTextureFromFile(lRenderManager.GetDevice(), lRenderManager.GetDeviceContext(), lTextureFileNameWString.c_str(), &lResource, &m_pTexture);
	
	if (lHR == E_FAIL)
		return false;
	
	ID3D11Texture2D * lTex2D= static_cast<ID3D11Texture2D *>(lResource);
	D3D11_TEXTURE2D_DESC desc;
	lTex2D->GetDesc(&desc);
	mSize.x = desc.Width;
	mSize.y = desc.Height;
	return true;
}

void CTexture::Bind(uint32 aStageId, ID3D11DeviceContext*  aContext)
{
	aContext->PSSetSamplers	(aStageId, 1, &m_SamplerState);
	aContext->PSSetShaderResources(aStageId, 1, &m_pTexture);
}

bool CTexture::Reload()
{
	//Destroy();
	//return Load(l_RenderManager);
	return true;
}

//delete[] m_pTexture;
