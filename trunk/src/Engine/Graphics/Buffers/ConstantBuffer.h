#pragma once
#ifndef _ENGINE_CONSTANTBUFFER_20170118_H
#define _ENGINE_CONSTANTBUFFER_20170118_H

#include "Graphics/Mesh/Buffer.h"
#include "Utils/Logger.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif


class CRenderManager;

class CConstantBuffer : public CBuffer
{
public:
    CConstantBuffer(CRenderManager& RenderManager, uint32 aByteWidth);
    CConstantBuffer(CRenderManager& RenderManager, void* aRawData, uint32 aByteWidth);
    virtual ~CConstantBuffer();

    virtual void Bind(ID3D11DeviceContext* aContext);
    virtual void BindVS(ID3D11DeviceContext* aContext, uint32 aBufferID);
    virtual void BindPS(ID3D11DeviceContext* aContext, uint32 aBufferID);
    virtual void Update(ID3D11DeviceContext* aContext, void* aRawData);

    void * GetRawData() const
    {
        return m_RawData;
    }

protected:
    void *m_RawData;
};

#endif //_ENGINE_CONSTANTBUFFER_20170118_H