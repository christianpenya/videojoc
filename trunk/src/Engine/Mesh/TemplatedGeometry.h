#pragma once
#ifndef _ENGINE_TEMPLATEDGEOMETRY_20161222
#define _ENGINE_TEMPLATEDGEOMETRY_20161222

#include "Geometry.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

template< class TVertexType >
class CTemplatedGeometry : public CGeometry
{
public:
    CTemplatedGeometry(CVertexBuffer<TVertexType>* VertexBuffer, D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology):
        CGeometry(PrimitiveTopology),
        m_VertexBuffer(VertexBuffer)
    {
    }

    virtual ~CTemplatedGeometry()
    {
        base::utils::CheckedDelete(m_VertexBuffer);
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

GEOMETRY_DEFINITION(CGeometryLinesList, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
GEOMETRY_DEFINITION(CGeometryTriangleList, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
GEOMETRY_DEFINITION(CGeometryTriangleStrip, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

#endif //_ENGINE_TEMPLATEDGEOMETRY_20161222
