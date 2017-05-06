#include "Mesh.h"
#include "Utils\BinFileReader.h"
#include "Engine\Engine.h"
#include "Render\RenderManager.h"
#include "VertexTypes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <memory>
#include "Graphics/Materials/Material.h"
#include "Graphics/Materials/MaterialManager.h"
#include "Graphics/Mesh/TemplatedIndexedGeometry.h"
#include "Graphics/Buffers/ConstantBufferManager.h"

CMesh::CMesh():
    mCount(0)
{}

CMesh::~CMesh()
{
    base::utils::CheckedDelete(mGeometries);
}

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
    else if (aVertexFlags == VertexTypes::ParticleVertex::GetVertexFlags())
    {
        return new CIndexedGeometryTriangleList<VertexTypes::ParticleVertex>
               (
                   new CVertexBuffer<VertexTypes::ParticleVertex>(aRM, aVertexData, aNumVertices),
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

                    if (lVertexFlags == VertexTypes::PositionBumpUV::GetVertexFlags())
                    {
                        unsigned short* IdxsData = (unsigned short*) lIndexData;
                        size_t lVertexStride = sizeof(VertexTypes::PositionBumpUV);
                        size_t lGeometryStride = 0;
                        size_t lNormalStride = sizeof(Vect3f);
                        size_t lTangentStride = lNormalStride + sizeof(Vect3f);
                        size_t lBiNormalStride = lTangentStride + sizeof(Vect4f);
                        size_t TextureCoordsStride = lBiNormalStride + sizeof(Vect4f);

                        for (size_t i = 0; i < lNumIndex; ++i)
                        {
                            if (IdxsData[i] >= lNumVertices)
                                i = i;
                        }

                        CalcTangentsAndBinormals(lVertexData, (unsigned short *)lIndexData, lNumVertices, lNumIndex, lVertexStride, lGeometryStride,
                                                 lNormalStride, lTangentStride, lBiNormalStride, TextureCoordsStride);
                    }

                    if (lNumVertices > 0 && lNumIndex > 0)
                    {
                        CGeometry* lGeometry = CreateGeometry(lRM, lVertexFlags, lNumVertices, lNumIndex, lVertexData, lIndexData);
                        mGeometries.push_back(lGeometry);
                    }

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
                base::utils::CheckedDelete(lAABB);

                //Bounding Sphere
                Vect3f lCenter = lBinFileReader->Read<Vect3f>();
                //#PARANORMAL no lee bien un float solo
                Vect3f lRadius = lBinFileReader->Read<Vect3f>();

                CBoundingSphere* lBoundingSphere = new CBoundingSphere();
                lBoundingSphere->SetRadius(lRadius.x);
                lBoundingSphere->SetCenter(lCenter);

                mBoundingSphere = *lBoundingSphere;
                base::utils::CheckedDelete(lBoundingSphere);
            }
        }

        unsigned short lFooter = lBinFileReader->Read<unsigned short>();
        lOk &= lFooter == FOOTER;

        lBinFileReader->Close();
    }
    else
    {
        lOk = false;
    }

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

void CMesh::CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t VtxCount,
                                     size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride, size_t
                                     TangentStride, size_t BiNormalStride, size_t TextureCoordsStride)
{
    Vect3f *tan1 = new Vect3f[VtxCount * 2];
    Vect3f *tan2 = tan1 + VtxCount;
    ZeroMemory(tan1, VtxCount * sizeof(Vect3f) * 2);
    unsigned char *l_VtxAddress = (unsigned char *)VtxsData;

    for (size_t b = 0; b<IdxCount; b += 3)
    {
        unsigned short i1 = IdxsData[b];
        unsigned short i2 = IdxsData[b + 1];
        unsigned short i3 = IdxsData[b + 2];

        if (i1 >= VtxCount)
            i1 = i1;
        if (i2 >= VtxCount)
            i1 = i1;
        if (i3 >= VtxCount)
            i1 = i1;

        Vect3f *v1 = (Vect3f *)&l_VtxAddress[i1*VertexStride + GeometryStride];
        Vect3f *v2 = (Vect3f *)&l_VtxAddress[i2*VertexStride + GeometryStride];
        Vect3f *v3 = (Vect3f *)&l_VtxAddress[i3*VertexStride + GeometryStride];
        Vect2f *w1 = (Vect2f
                      *)&l_VtxAddress[i1*VertexStride + TextureCoordsStride];
        Vect2f *w2 = (Vect2f
                      *)&l_VtxAddress[i2*VertexStride + TextureCoordsStride];
        Vect2f *w3 = (Vect2f
                      *)&l_VtxAddress[i3*VertexStride + TextureCoordsStride];

        float x1 = v2->x - v1->x;
        float x2 = v3->x - v1->x;
        float y1 = v2->y - v1->y;
        float y2 = v3->y - v1->y;
        float z1 = v2->z - v1->z;
        float z2 = v3->z - v1->z;
        float s1 = w2->x - w1->x;
        float s2 = w3->x - w1->x;
        float t1 = w2->y - w1->y;
        float t2 = w3->y - w1->y;
        float r = 1.0F / (s1 * t2 - s2 * t1);

        Vect3f sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                    (t2 * z1 - t1 * z2) * r);

        Vect3f tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                    (s1 * z2 - s2 * z1) * r);

        assert(i1<VtxCount);
        assert(i2<VtxCount);
        assert(i3<VtxCount);

        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;
        tan2[i1] += tdir;
        tan2[i2] += tdir;
        tan2[i3] += tdir;
    }

    for (size_t b = 0; b<VtxCount; ++b)
    {
        Vect3f *l_NormalVtx = (Vect3f*)&l_VtxAddress[b*VertexStride + NormalStride];
        Vect3f *l_TangentVtx = (Vect3f*)&l_VtxAddress[b*VertexStride + TangentStride];
        Vect4f *l_TangentVtx4 = (Vect4f*)&l_VtxAddress[b*VertexStride + TangentStride];
        Vect3f *l_BiNormalVtx = (Vect3f*)&l_VtxAddress[b*VertexStride + BiNormalStride];
        const Vect3f& t = tan1[b];

        // Gram-Schmidt orthogonalize
        Vect3f l_VAl = t - (*l_NormalVtx)*((*l_NormalVtx)*t);
        l_VAl.Normalize();
        *l_TangentVtx = l_VAl;

        // Calculate handedness
        Vect3f l_Cross;
        l_Cross = (*l_NormalVtx) ^ (*l_TangentVtx);
        l_TangentVtx4->w = (l_Cross*(tan2[b])) < 0.0f ? -1.0f : 1.0f;
        *l_BiNormalVtx = (*l_NormalVtx) ^ (*l_TangentVtx);
    }

    delete[] tan1;
}

void CMesh::DrawImGui()
{
    for (size_t i = 0, lCount = mGeometries.size(); i < lCount; ++i)
    {
        mMaterials[i]->DrawImgui();
    }
}