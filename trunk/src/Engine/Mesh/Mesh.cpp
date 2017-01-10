#include "Mesh.h"
#include "Base\Utils\BinFileReader.h"
#include "Engine\Engine.h"
#include "Engine\Render\RenderManager.h"
#include "VertexTypes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <iostream>
#include <memory>
#include "Engine/Materials/Material.h"
#include "Engine/Materials/MaterialManager.h"
#include "Mesh/TemplatedIndexedGeometry.h"

CGeometry* CreateGeometry(CRenderManager& aRM, uint32& aVertexFlags, uint32& aNumVertices, void* aVertexData, void* aIndexData)
{

    if (aVertexFlags == VertexTypes::PositionUV::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::PositionUV>
               (
                   new CVertexBuffer<VertexTypes::PositionUV>(aRM, aVertexData, aNumVertices),
                   new CIndexBuffer(aRM, aIndexData, aNumVertices, 16)
               );
    }
    else if (aVertexFlags == VertexTypes::PositionNormal::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::PositionNormal>
               (
                   new CVertexBuffer<VertexTypes::PositionNormal>(aRM, aVertexData, aNumVertices),
                   new CIndexBuffer(aRM, aIndexData, aNumVertices, 16)
               );
    }
    else if (aVertexFlags == VertexTypes::PositionNormalUV::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::PositionNormalUV>
               (
                   new CVertexBuffer<VertexTypes::PositionNormalUV>(aRM, aVertexData, aNumVertices),
                   new CIndexBuffer(aRM, aIndexData, aNumVertices, 16)
               );
    }
    else if (aVertexFlags == VertexTypes::PositionBump::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::PositionBump>
               (
                   new CVertexBuffer<VertexTypes::PositionBump>(aRM, aVertexData, aNumVertices),
                   new CIndexBuffer(aRM, aIndexData, aNumVertices, 16)
               );
    }
    else if (aVertexFlags == VertexTypes::PositionBumpUV::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::PositionBumpUV>
               (
                   new CVertexBuffer<VertexTypes::PositionBumpUV>(aRM, aVertexData, aNumVertices),
                   new CIndexBuffer(aRM, aIndexData, aNumVertices, 16)
               );
    }
    else if (aVertexFlags == VertexTypes::PositionBumpUVUV2::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::PositionBumpUVUV2>
               (
                   new CVertexBuffer<VertexTypes::PositionBumpUVUV2>(aRM, aVertexData, aNumVertices),
                   new CIndexBuffer(aRM, aIndexData, aNumVertices, 16)
               );
    }
}

bool CMesh::Load(const std::string& aFilename)
{
    bool lOk = false;

    std::shared_ptr<base::utils::CBinFileReader> lBinFileReader = std::make_shared<base::utils::CBinFileReader>(aFilename);
    lBinFileReader->Open();

    // read header
    unsigned short lHeader = lBinFileReader->Read<unsigned short>();

    CRenderManager& lRM = CEngine::GetInstance().GetRenderManager();
    CMaterialManager& lMM = CEngine::GetInstance().GetMaterialManager();

    if (lHeader == HEADER)
    {
        unsigned short lNumMaterialesMallas = lBinFileReader->Read<unsigned short>();
        for (unsigned short iMatMesh = 0; iMatMesh < lNumMaterialesMallas; ++iMatMesh)
        {
            //MATERIAL
            std::string lMaterialName = lBinFileReader->Read<std::string>();
            mMaterials.push_back(lMM(lMaterialName));

            //VERTEX
            uint32 lVertexFlags = lBinFileReader->Read<uint32>();
            uint32 lNumVertices = lBinFileReader->Read<uint32>();

            uint32 lVertexSize = VertexTypes::GetVertexSize(lVertexFlags);
            size_t lNumBytesVertices = lNumVertices * lVertexSize;
            void* lVertexData = lBinFileReader->ReadRaw(lNumBytesVertices);

            //INDEX
            size_t lNumBytesIndexes = lNumVertices * sizeof(unsigned int);
            void* lIndexData = lBinFileReader->ReadRaw(lNumBytesVertices);

            CGeometry* lGeometry = CreateGeometry(lRM, lVertexFlags, lNumVertices, lVertexData, lIndexData);
            mGeometries.push_back(lGeometry);

            free(lVertexData);
            free(lIndexData);

            //AABB
            Vect3f lMinVertex = lBinFileReader->Read<Vect3f>();
            Vect3f lMaxVertex = lBinFileReader->Read<Vect3f>();

            CAxisAlignedBoundingBox* lAABB = new CAxisAlignedBoundingBox();
            lAABB->SetMin(lMinVertex);
            lAABB->SetMax(lMaxVertex);

            mAABB = *lAABB;

            //Bounding Sphere
            float lRadius = lBinFileReader->Read<float>();
            Vect3f lCenter = lBinFileReader->Read<Vect3f>();

            CBoundingSphere* lBoundingSphere = new CBoundingSphere();
            lBoundingSphere->SetRadius(lRadius);
            lBoundingSphere->SetCenter(lCenter);

            mBoundingSphere = *lBoundingSphere;
        }
    }

    lBinFileReader->Close();

    lOk &= lHeader == FOOTER;

    return lOk;
}