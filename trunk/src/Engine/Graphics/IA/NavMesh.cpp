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

{

    std::string lObj = aTreeNode->GetAttribute<std::string>("obj", "");

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, lObj.c_str(),"",true);

    if (!err.empty())
        printf("error obj");

    if (!ret)
        printf("Failed to load/parse .obj.\n");

    for (size_t v = 0; v < attrib.vertices.size(); v += 3)
    {
        Vertex lvertex;
        lvertex.position = Vect3f(attrib.vertices[v], attrib.vertices[v + 1], attrib.vertices[v+2]);
        m_Vertexs.push_back(lvertex);
    }

    std::vector<uint16> triangles;


    int i = 0;
    for (size_t v = 0; v < shapes[0].mesh.indices.size(); v+=3)
    {
        Triangle ltriangle;
        ltriangle.id = i;
        ltriangle.idx1 = shapes[0].mesh.indices[v].vertex_index;
        ltriangle.idx2 = shapes[0].mesh.indices[v+1].vertex_index;
        ltriangle.idx3 = shapes[0].mesh.indices[v+2].vertex_index;
        m_Triangles.push_back(ltriangle);

        triangles.push_back(ltriangle.idx1);
        triangles.push_back(ltriangle.idx2);
        triangles.push_back(ltriangle.idx3);

        ++i;
    }

    int valor = 0;
    int valor2 = 0;
    for (size_t v = 0; v < attrib.normals.size(); v += 3)
        m_Triangles[attrib.normals[v]-1].m_neighbour.push_back(*GetTriangle(attrib.normals[v + 1]-1));

    /*
    //ACTIVAR PARA GRAFICAR
    CRenderManager& lRM = CEngine::GetInstance().GetRenderManager();
    CMaterialManager& lMM = CEngine::GetInstance().GetMaterialManager();

    CGeometry* lgeometry = new CIndexedGeometryTriangleList<VertexTypes::Position>
    (
        new CVertexBuffer<VertexTypes::Position>(lRM, attrib.vertices.data(), attrib.vertices.size()/3),
        new CIndexBuffer(lRM, triangles.data(), triangles.size(), 16)
    );

    mGeometries = lgeometry;
    mMaterials = lMM(aTreeNode->GetAttribute<std::string>("material", ""));
    */

}


bool CNavMesh::Render(CRenderManager& lRM)
{

    bool lOk = true;
    /*
    //ACTIVAR PARA GRAFICAR
    mMaterials->Apply();
    mGeometries->RenderIndexed(lRM.GetDeviceContext());
    */
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
    if (ImGui::TreeNode("NavMesh"))
    {
        ImGui::TreePop();
    }


}

void CNavMesh::Deactivate()
{
}