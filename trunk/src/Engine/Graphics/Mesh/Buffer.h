#pragma once
#ifndef _ENGINE_BUFFER_20161222
#define _ENGINE_BUFFER_20161222

#include "Utils\Defines.h"
#include <d3d11.h>

class CBuffer
{
public:
    enum BufferUsage
    {
        eDefault = 0,
        eImmutable,
        eDynamic,
        eStaging
    };

    CBuffer() : m_pBuffer(nullptr) {}
    virtual ~CBuffer() {}
    GET_SET_PTR(ID3D11Buffer, Buffer);
    virtual void Update(ID3D11DeviceContext*, void*) {}
    virtual void Bind(ID3D11DeviceContext*) = 0;

protected:
    ID3D11Buffer* m_pBuffer;

};

#endif //_ENGINE_BUFFER_20161222