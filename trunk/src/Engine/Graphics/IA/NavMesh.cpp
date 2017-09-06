#include "NavMesh.h"
#include "Graphics/Effects/Technique.h"
#include "Graphics/Effects/EffectManager.h"
#include "NavMeshManager.h"
#include "Engine/engine.h"
#include "XML\XML.h"
#include "Graphics\Mesh\TemplatedGeometry.h"
#include "Graphics\Mesh\VertexBuffer.h"
#include "Graphics\Buffers\ConstantBufferManager.h"
#include "Graphics\Mesh\Mesh.h"
#include "Graphics\Mesh\TemplatedIndexedGeometry.h"
#include "Graphics\Materials\MaterialManager.h"
#include "Graphics\IA\tiny_obj_loader.h"
#include "Pathfinding.h"

CNavMesh::CNavMesh()
{

}

CNavMesh::~CNavMesh()
{

}


CNavMesh::CNavMesh(const CXMLElement* aTreeNode)
    : CSceneNode(aTreeNode)
    , m_Color(aTreeNode->GetAttribute<CColor>("color", CColor(1.0f, 0.0f, 0.0f, 1.0f)))

{}


bool CNavMesh::Render(CRenderManager& lRM)
{

    bool lOk = true;
    mMaterials->Apply();
    mGeometries->RenderIndexed(lRM.GetDeviceContext());

    /*CPathfinding* lpath = new CPathfinding(Vect3f(-55.5f, 0.01214442f, 41.7f), Vect3f(-42.66667f, 0.01214442f, 42.66667f), "navMeshScene01");
    lpath->GetPath();
    bool encontro = lpath->PathfindStep();


    int indice = atTriangle(Vect3f(-55.36667f,0.01214442f, 44.16667f));
    Triangle* l_triangle = GetTriangle(indice);
    std::string temp = "En triangulo: " + indice;
    LOG_INFO_APPLICATION("triangle ", temp);*/
    return lOk;
}

Vect3f CNavMesh::GetCenter(Triangle* triangle)
{

    Vertex* l_vertex1 = GetVertex(triangle->idx1);
    Vertex* l_vertex2 = GetVertex(triangle->idx2);
    Vertex* l_vertex3 = GetVertex(triangle->idx3);

    return (l_vertex1->position + l_vertex2->position + l_vertex3->position) / 3;
}


int CNavMesh::atTriangle(Vect3f position)
{
    Vertex* l_vertex1;
    Vertex* l_vertex2;
    Vertex* l_vertex3;

    float v1, v2, v3;

    for (size_t i = 0; i < m_Triangles.size(); ++i)
    {
        l_vertex1 = GetVertex(m_Triangles[i].idx1);
        l_vertex2 = GetVertex(m_Triangles[i].idx2);
        l_vertex3 = GetVertex(m_Triangles[i].idx3);

        v1 = (l_vertex1->position.x - position.x)*(l_vertex2->position.z - position.z) - (l_vertex2->position.x - position.x)*(l_vertex1->position.z - position.z);
        v2 = (l_vertex2->position.x - position.x)*(l_vertex3->position.z - position.z) - (l_vertex3->position.x - position.x)*(l_vertex2->position.z - position.z);
        v3 = (l_vertex3->position.x - position.x)*(l_vertex1->position.z - position.z) - (l_vertex1->position.x - position.x)*(l_vertex3->position.z - position.z);

        if (((v1 >= 0) && (v2 >= 0) && (v3 >= 0)) || ((v1 <= 0) && (v2 <= 0) && (v3 <= 0)))
            return i;
    }
    return -1;
}

void CNavMesh::DrawImgui()
{

}

void CNavMesh::Deactivate()
{
}