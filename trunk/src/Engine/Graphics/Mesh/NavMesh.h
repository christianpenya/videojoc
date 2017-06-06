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

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CNavMesh : public CSceneNode
{
private:
    struct TriangleData
    {
        Vect3f PositionV1, PositionV2, PositionV3;
        CColor Color;
        float Size;
    };

    TriangleData m_TriangleData;
    CGeometryTriangleList<VertexTypes::ParticleVertex> * m_Vertices;

public:
    CNavMesh();
    virtual ~CNavMesh();
    CNavMesh(const CXMLElement* aElement);

    VertexTypes::ParticleVertex m_TriangleRenderableData;

    bool Render(CRenderManager& lRM);
    bool Update(float ElapsedTime);
    void DrawImgui();
};


#endif //_NAVMESH_H