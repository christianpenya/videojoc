#include "NavMesh.h"
#include "Graphics/Effects/Technique.h"
#include "Graphics/Effects/EffectManager.h"
#include "NavMeshManager.h"
#include "Engine/engine.h"
#include "XML\XML.h"
#include "Graphics\Mesh\TemplatedGeometry.h"
#include "Graphics\Mesh\VertexBuffer.h"
#include "Graphics\Buffers\ConstantBufferManager.h"

CNavMesh::CNavMesh()
{

}

CNavMesh::~CNavMesh()
{

}


CNavMesh::CNavMesh(const CXMLElement* aElement)
    : CSceneNode(aElement)
{

    m_TriangleData.PositionV1 = aElement->GetAttribute<Vect3f>("vertice1Pos", Vect3f(0.0f, 0.0f, 0.0f));
    m_TriangleData.PositionV2 = aElement->GetAttribute<Vect3f>("vertice2Pos", Vect3f(0.0f, 0.0f, 0.0f));
    m_TriangleData.PositionV3 = aElement->GetAttribute<Vect3f>("vertice3Pos", Vect3f(0.0f, 0.0f, 0.0f));
    m_TriangleData.Color = aElement->GetAttribute<CColor>("color", CColor(1.0f, 0.0f, 0.0f, 1.0f));
    m_TriangleData.Size = aElement->GetAttribute<float>("size", 0.5f);

    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();
    //CVertexBuffer<VertexTypes::ParticleVertex> *l_buffer = new CVertexBuffer < VertexTypes::ParticleVertex>(lRenderManager, m_TriangleData, 1);
    //m_Vertices = new CGeometryTriangleList<VertexTypes::ParticleVertex>(l_buffer);
}

/*
bool CNavMesh::Render(CRenderManager& lRM)
{

    m_TriangleRenderableData.position = m_TriangleData.PositionV1;
    m_TriangleRenderableData.color = m_TriangleData.Color;
    m_TriangleRenderableData.uv.x = m_TriangleData.Size;
    m_TriangleRenderableData.uv.y = 0;
    m_TriangleRenderableData.uv2.x = 0;
    m_TriangleRenderableData.uv2.y = 0;

    CConstantBufferManager& lCBM = CEngine::GetInstance().GetConstantBufferManager();
    lCBM.mFrameDesc.m_ViewProjection = lRM.GetViewProjectionMatrix();
    lCBM.mFrameDesc.m_View = lRM.GetViewMatrix();
    lCBM.mFrameDesc.m_Projection = lRM.GetProjectionMatrix();
    lCBM.mObjDesc.m_World = GetMatrix();

    lCBM.BindBuffer(lRM.GetDeviceContext(), CConstantBufferManager::CB_Object);
    lCBM.BindBuffer(lRM.GetDeviceContext(), CConstantBufferManager::CB_Frame);

    m_Vertices->UpdateVertex(&m_TriangleRenderableData,1);
    m_Vertices->Render(lRM.GetDeviceContext(), 1);
    lRM.GetDeviceContext()->GSSetShader(nullptr, nullptr, 0);

    return true;
	}*/