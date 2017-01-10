#pragma once
#ifndef _ENGINE_INDEXBUFFER_20161214
#define _ENGINE_INDEXBUFFER_20161214

#include"Buffer.h"
#include "Engine\Render\RenderManager.h"

class CIndexBuffer : public CBuffer {

public:
	CIndexBuffer(CRenderManager& RenderManager, void* aRawData, uint32 aNumIndices, uint16
		IndexType)
		: mNumIndices(aNumIndices)
	{
		if (IndexType == 16)
			mIndexType = DXGI_FORMAT_R16_UINT;
		else
			mIndexType = DXGI_FORMAT_R32_UINT;
	
        D3D11_BUFFER_DESC l_IndexBuffer;
		ZeroMemory(&l_IndexBuffer, sizeof(l_IndexBuffer));
		l_IndexBuffer.Usage = D3D11_USAGE_DEFAULT;
		l_IndexBuffer.ByteWidth = ((DXGI_FORMAT_R16_UINT == mIndexType) ? sizeof(WORD) : sizeof(unsigned int)) * mNumIndices;
		l_IndexBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
		l_IndexBuffer.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = aRawData;
		ID3D11Device *lDevice = RenderManager.GetDevice();
		HRESULT l_HR = lDevice->CreateBuffer(&l_IndexBuffer, &InitData, &m_pBuffer);
		assert(SUCCEEDED(l_HR));
	}

	virtual ~CIndexBuffer(){}

	virtual void Bind(ID3D11DeviceContext* aContext)
	{
		aContext->IASetIndexBuffer(m_pBuffer, mIndexType, 0);
	}
	
    inline DXGI_FORMAT GetIndexType() const { return mIndexType; }
	inline uint32 GetIndexCount() const { return mNumIndices; }

protected:
	DXGI_FORMAT mIndexType;
	uint32 mNumIndices;
};

#endif //_ENGINE_INDEXBUFFER_20161214
