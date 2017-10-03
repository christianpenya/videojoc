#pragma once
#ifndef _NAVMESH_H
#define _NAVMESH_H

#include "Math/Matrix44.h"
#include "Math/Color.h"
#include "XML\XML.h"
#include "Utils/Logger.h"
#include "Graphics/Mesh/VertexTypes.h"
#include "Graphics/Mesh/TemplatedGeometry.h"
#include "Graphics/Mesh/VertexBuffer.h"
#include "Graphics/Scenes/SceneNode.h"
#include "d3dcompiler.h"
#include "d3d11.h"
#include "Graphics/Materials/Material.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif


class CNavMesh : public CSceneNode
{

public:

    /*    struct Triangle
        {
            struct Edge
            {
                int m_verticesID[2];
                Triangle * m_neighbour;
            };
            uint16_t m_id;
            std::vector<Vect3f> m_vertices;
            std::vector<Edge> m_edges;
        };
    	*/

    struct Triangle
    {
        int id;
        uint16 idx1, idx2, idx3; //Vertex pos in array
        std::vector<Triangle> m_neighbour;
    };

    struct Vertex
    {
        Vect3f position;
    };

    CColor m_Color;
    std::vector<Vertex> m_Vertexs;
    std::vector<Triangle> m_Triangles;

    CGeometry* mGeometries;
    CMaterial* mMaterials;

    Triangle * GetTriangle(uint16_t index)
    {
        return &(m_Triangles.at(index));
    }

    Vertex * GetVertex(uint16_t index)
    {
        return &(m_Vertexs.at(index));
    }

    uint16_t GetNumTriangles()
    {
        return m_Triangles.size();
    }

    std::vector<Triangle> GetNeighbours(Triangle* ltriangle)
    {
        return ltriangle->m_neighbour;
    }


public:
    CNavMesh();
    virtual ~CNavMesh();
    CNavMesh(const CXMLElement* aElement);


    int atTriangle(Vect3f position);
    Vect3f GetCenter(Triangle* triangle);
    bool Render(CRenderManager& lRM);
    void DrawImgui();
    void Deactivate();
};


#endif //_NAVMESH_H