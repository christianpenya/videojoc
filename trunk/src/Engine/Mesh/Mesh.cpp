#include "Mesh.h"
#include "Utils\BinFileReader.h"
#include "Engine\Engine.h"
#include "Render\RenderManager.h"
#include "VertexTypes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <memory>
#include "Materials/Material.h"
#include "Materials/MaterialManager.h"
#include "Mesh/TemplatedIndexedGeometry.h"
#include "Scenes/ConstantBufferManager.h"

CMesh::CMesh() {}
CMesh::~CMesh() {}

CGeometry* CreateGeometry(CRenderManager& aRM, unsigned short aVertexFlags, unsigned short aNumVertices, unsigned short aNumIndices, void* aVertexData, void* aIndexData)
{
    if (aVertexFlags == VertexTypes::PositionUV::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::PositionUV>
               (
                   new CVertexBuffer<VertexTypes::PositionUV>(aRM, aVertexData, aNumVertices),
                   new CIndexBuffer(aRM, aIndexData, aNumIndices, 16)
               );
    }
    else if (aVertexFlags == VertexTypes::PositionNormal::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::PositionNormal>
               (
                   new CVertexBuffer<VertexTypes::PositionNormal>(aRM, aVertexData, aNumVertices),
                   new CIndexBuffer(aRM, aIndexData, aNumIndices, 16)
               );
    }
    else if (aVertexFlags == VertexTypes::PositionNormalUV::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::PositionNormalUV>
               (
                   new CVertexBuffer<VertexTypes::PositionNormalUV>(aRM, aVertexData, aNumVertices),
                   new CIndexBuffer(aRM, aIndexData, aNumIndices, 16)
               );
    }
    else if (aVertexFlags == VertexTypes::PositionNormalUVUV2::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::PositionNormalUVUV2>
               (
                   new CVertexBuffer<VertexTypes::PositionNormalUVUV2>(aRM, aVertexData, aNumVertices),
                   new CIndexBuffer(aRM, aIndexData, aNumIndices, 16)
               );
    }
    else if (aVertexFlags == VertexTypes::PositionBump::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::PositionBump>
               (
                   new CVertexBuffer<VertexTypes::PositionBump>(aRM, aVertexData, aNumVertices),
                   new CIndexBuffer(aRM, aIndexData, aNumIndices, 16)
               );
    }
    else if (aVertexFlags == VertexTypes::PositionBumpUV::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::PositionBumpUV>
               (
                   new CVertexBuffer<VertexTypes::PositionBumpUV>(aRM, aVertexData, aNumVertices),
                   new CIndexBuffer(aRM, aIndexData, aNumIndices, 16)
               );
    }
    else if (aVertexFlags == VertexTypes::PositionBumpUVUV2::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::PositionBumpUVUV2>
               (
                   new CVertexBuffer<VertexTypes::PositionBumpUVUV2>(aRM, aVertexData, aNumVertices),
                   new CIndexBuffer(aRM, aIndexData, aNumIndices, 16)
               );
    }

    return false;
}

bool CMesh::Load(const std::string& aFilename)
{
    m_Name = aFilename;
    bool lOk = true;

    std::shared_ptr<base::utils::CBinFileReader> lBinFileReader = std::make_shared<base::utils::CBinFileReader>(aFilename);
    if (lBinFileReader->Open())
    {
        unsigned short lHeader = lBinFileReader->Read<unsigned short>();

        if (lHeader == HEADER)
        {
            CRenderManager& lRM = CEngine::GetInstance().GetRenderManager();
            CMaterialManager& lMM = CEngine::GetInstance().GetMaterialManager();

            unsigned short lNumMaterialesMallas = lBinFileReader->Read<unsigned short>();
            if (lNumMaterialesMallas > 0)
            {
                mMaterials.resize(lNumMaterialesMallas);
                mGeometries.reserve(lNumMaterialesMallas);
                for (unsigned short iMatMesh = 0; iMatMesh < lNumMaterialesMallas; ++iMatMesh)
                {
                    //MATERIAL
                    std::string lMaterialName = lBinFileReader->Read<std::string>();
                    mMaterials[iMatMesh] = lMM(lMaterialName);

                    //VERTEX
                    unsigned short lVertexFlags = lBinFileReader->Read<unsigned short>();
                    unsigned short lNumVertices = lBinFileReader->Read<unsigned short>();

                    uint32 lVertexSize = VertexTypes::GetVertexSize(lVertexFlags);
                    uint32 lNumBytesVertices = lNumVertices * lVertexSize;
                    void* lVertexData = lBinFileReader->ReadRaw(lNumBytesVertices);
                    //inspect debug example: (float*)lVertexData,24

                    //INDEX
                    unsigned short lNumIndex = lBinFileReader->Read<unsigned short>();
                    size_t lNumBytesIndexes = lNumIndex * sizeof(unsigned short);
                    void* lIndexData = lBinFileReader->ReadRaw(lNumBytesIndexes);

                    CGeometry* lGeometry = CreateGeometry(lRM, lVertexFlags, lNumVertices, lNumIndex, lVertexData, lIndexData);
                    mGeometries.push_back(lGeometry);

                    free(lVertexData);
                    free(lIndexData);
                }

                //AABB
                Vect3f lMinVertex = lBinFileReader->Read<Vect3f>();
                Vect3f lMaxVertex = lBinFileReader->Read<Vect3f>();

                CAxisAlignedBoundingBox* lAABB = new CAxisAlignedBoundingBox();
                lAABB->SetMin(lMinVertex);
                lAABB->SetMax(lMaxVertex);

                mAABB = *lAABB;

                //Bounding Sphere
                Vect3f lCenter = lBinFileReader->Read<Vect3f>();
                //#PARANORMAL no lee bien un float solo
                Vect3f lRadius = lBinFileReader->Read<Vect3f>();

                CBoundingSphere* lBoundingSphere = new CBoundingSphere();
                lBoundingSphere->SetRadius(lRadius.x);
                lBoundingSphere->SetCenter(lCenter);

                mBoundingSphere = *lBoundingSphere;
            }
        }

        unsigned short lFooter = lBinFileReader->Read<unsigned short>();
        lOk &= lFooter == FOOTER;

        lBinFileReader->Close();
        return lOk;
    }
    else
        lOk = false;

    return lOk;
}

bool CMesh::Render(CRenderManager& aRM)
{
    bool lOk = true;

    for (size_t i = 0, lCount = mGeometries.size(); i < lCount; ++i)
    {
        mMaterials[i]->Apply();
        mGeometries[i]->RenderIndexed(aRM.GetDeviceContext());
    }

    return lOk;
}