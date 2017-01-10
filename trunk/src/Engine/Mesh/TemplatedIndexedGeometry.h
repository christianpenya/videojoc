#pragma once
#ifndef _ENGINE_TEMPLATEDINDEXEDDGEOMETRY_20161222
#define _ENGINE_TEMPLATEDINDEXEDDGEOMETRY_20161222

#include "Geometry.h"

template< class TVertexType >
class CTemplatedIndexedGeometry : public CGeometry
{
public:
    CTemplatedIndexedGeometry(CVertexBuffer<TVertexType>* aVertexBuffer, CIndexBuffer* aIndexBuffer, D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology) :
        CGeometry(PrimitiveTopology),
        m_VertexBuffer(aVertexBuffer),
        m_IndexBuffer(aIndexBuffer)
    {
    }

    virtual ~CTemplatedIndexedGeometry()
    {
        base::utils::CheckedDelete(m_VertexBuffer);
        base::utils::CheckedDelete(m_IndexBuffer);
    }

    virtual bool RenderIndexed(ID3D11DeviceContext* aContext, uint32 IndexCount = -1, uint32 StartIndexLocation = 0, uint32 BaseVertexLocation = 0)
    {
        // Send the vertex buffer and index buffer to the GPU
        m_VertexBuffer->Bind(aContext);
        m_IndexBuffer->Bind(aContext);

        // Configure the type of topology to be renderer ( p.e. Triangles, Quads, points,... )
        aContext->IASetPrimitiveTopology(m_PrimitiveTopology);

        // Finally draw the geometry
        aContext->DrawIndexed(IndexCount == -1 ? m_IndexBuffer->GetIndexCount() : IndexCount, StartIndexLocation, BaseVertexLocation);

        return true;
    }

private:
    CIndexBuffer*  m_IndexBuffer;
    CVertexBuffer<TVertexType> * m_VertexBuffer;
};


#define DEFINE_TOPOLGY_INDEXED_GEOMETRY( ClassName, TopologyType ) \
template < class T >\
class ClassName : public CTemplatedIndexedGeometry< T > \
{\
public:\
ClassName(CVertexBuffer< T > * aVB, CIndexBuffer *aIB) \
: CTemplatedIndexedGeometry(aVB, aIB, TopologyType) \
{} \
virtual ~ClassName()\
{} \
};

DEFINE_TOPOLGY_INDEXED_GEOMETRY(CIndexedGeometryLineList, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
DEFINE_TOPOLGY_INDEXED_GEOMETRY(CIndexedGeometryTriangleList, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
DEFINE_TOPOLGY_INDEXED_GEOMETRY(CIndexedGeometryTriangleStrip, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

#endif //_ENGINE_TEMPLATEDINDEXEDDGEOMETRY_20161222