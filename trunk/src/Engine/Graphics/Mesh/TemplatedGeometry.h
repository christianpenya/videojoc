#pragma once
#ifndef _ENGINE_TEMPLATEDGEOMETRY_20161222
#define _ENGINE_TEMPLATEDGEOMETRY_20161222

#include "Geometry.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Utils/CheckedDelete.h"
#include "Graphics/Effects/GeometryShader.h"
#include "Graphics/Effects/PixelShader.h"
#include "Graphics/Effects/VertexShader.h"
#include "Engine/Engine.h"
#include "Render/RenderManager.h"


template< class TVertexType >
class CTemplatedGeometry : public CGeometry
{
public:
    CTemplatedGeometry(CVertexBuffer<TVertexType>* VertexBuffer, D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology):
        CGeometry(PrimitiveTopology),
        m_VertexBuffer(VertexBuffer)
    {}

    virtual ~CTemplatedGeometry()
    {
        base::utils::CheckedDelete(m_VertexBuffer);
    }

    virtual bool Render(ID3D11DeviceContext* aContext, int num = -1)
    {
        // Send the vertex buffer to the GPU
        m_VertexBuffer->Bind(aContext);

        // Configure the type of topology to be renderer ( p.e. Triangles, Quads, points,... )
        aContext->IASetPrimitiveTopology(m_PrimitiveTopology);

        // Finally draw the geometry
        aContext->Draw(num < 0 ? m_VertexBuffer->GetNumVertexs() : num, 0);

        return true;
    }


    virtual bool Render(ID3D11DeviceContext* aContext)
    {
        // Send the vertex buffer to the GPU
        m_VertexBuffer->Bind(aContext);

        // Configure the type of topology to be renderer ( p.e. Triangles, Quads, points,... )
        aContext->IASetPrimitiveTopology(m_PrimitiveTopology);

        // Finally draw the geometry
        aContext->Draw(m_VertexBuffer->GetNumVertexs(), 0);

        return true;
    }

    bool UpdateVertex(void *Vtxs, unsigned int VtxsCount)
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        ZeroMemory(&mappedResource, sizeof(mappedResource));

        ID3D11DeviceContext *l_DeviceContext = CEngine::GetInstance().GetRenderManager().GetDeviceContext();
        HRESULT l_HR = l_DeviceContext->Map(m_VertexBuffer->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        if (FAILED(l_HR))
            return false;

        memcpy(mappedResource.pData, Vtxs, sizeof(TVertexType) * VtxsCount);

        l_DeviceContext->Unmap(m_VertexBuffer->GetBuffer(), 0);

        return true;
    }


protected:
    CVertexBuffer<TVertexType> *m_VertexBuffer;
};

#define GEOMETRY_DEFINITION(ClassName, TopologyType) \
    template<class T> \
    class ClassName : public CTemplatedGeometry<T> \
    { \
    public: \
        ClassName(CVertexBuffer <T> * aVB) \
        : CTemplatedGeometry( aVB, TopologyType) \
        {} \
    }; \



GEOMETRY_DEFINITION(CGeometryPointList, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
GEOMETRY_DEFINITION(CGeometryLinesList, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
GEOMETRY_DEFINITION(CGeometryTriangleList, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
GEOMETRY_DEFINITION(CGeometryTriangleStrip, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

#endif //_ENGINE_TEMPLATEDGEOMETRY_20161222
