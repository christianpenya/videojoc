#pragma once
#ifndef _ENGINE_VERTEXBUFFER_20161222
#define _ENGINE_VERTEXBUFFER_20161222

#include "Buffer.h"
#include "Render\RenderManager.h"

template< class TVertexType >
class CVertexBuffer : public CBuffer
{
public:
    CVertexBuffer(CRenderManager& RenderManager, void* aRawData, uint32 aNumVertexs) :
        mNumVertexs(aNumVertexs)
    {
        D3D11_BUFFER_DESC lVertexBufferDesc;
        ZeroMemory(&lVertexBufferDesc, sizeof(lVertexBufferDesc));
        lVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        lVertexBufferDesc.ByteWidth = sizeof(TVertexType)*mNumVertexs;
        lVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        lVertexBufferDesc.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = aRawData;
        ID3D11Device *lDevice = RenderManager.GetDevice();
        HRESULT l_HR = lDevice->CreateBuffer(&lVertexBufferDesc, &InitData, &m_pBuffer);
        assert(SUCCEEDED(l_HR));
    }

    virtual ~CVertexBuffer() {}

    virtual void Bind(ID3D11DeviceContext* aContext)
    {
        uint32 offset = 0, stride = GetStride();
        aContext->IASetVertexBuffers(0, 1, &m_pBuffer, &stride, &offset);
    }

    inline uint32 GetNumVertexs() const
    {
        return mNumVertexs;
    }
    inline uint32 GetStride() const
    {
        return sizeof(TVertexType);
    }

protected:
    uint32 mNumVertexs;
};

#endif //_ENGINE_VERTEXBUFFER_20161222
