#pragma once
#ifndef _ENGINE_GEOMETRY_20161222
#define _ENGINE_GEOMETRY_20161222

#include<assert.h>
#include "d3d11.h"
#include "Math\MathUtils.h"
#include "Utils\CheckedDelete.h"

class CGeometry
{
public:

    CGeometry(D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology) : m_PrimitiveTopology(PrimitiveTopology) {}
    virtual ~CGeometry()
    {
    }

    virtual bool Render(ID3D11DeviceContext*, int num = -1)
    {
        assert(!"This method mustn't be called");
        return false;
    }

    virtual bool RenderIndexed(ID3D11DeviceContext*, uint32 IndexCount = -1, uint32 StartIndexLocation = 0, uint32 BaseVertexLocation = 0)
    {
        assert(!"This method mustn't be called");
        return false;
    }

protected:
    D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
};

#endif //_ENGINE_GEOMETRY_20161222
