#pragma once
#ifndef _ENGINE_MESH_20161222
#define _ENGINE_MESH_20161222

#include "Utils\Name.h"
#include <vector>
#include "BoundingSphere.h"
#include "AxisAlignedBoundingBox.h"

class CRenderManager;
class CMaterial;
class CGeometry;

class CMesh : public CName
{
public:
    CMesh();
    virtual ~CMesh();
    bool Load(const std::string& aFilename);
    bool Render(CRenderManager& aRenderManager);

protected:
    uint32 mCount;
    std::vector< CMaterial* > mMaterials;
    std::vector< CGeometry* > mGeometries;
    CAxisAlignedBoundingBox mAABB;
    CBoundingSphere mBoundingSphere;

public:

    CBoundingSphere GetBoundingSphere()
    {
        return mBoundingSphere;
    }

    CAxisAlignedBoundingBox GetAABB()
    {
        return mAABB;
    }

private:
    void CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t VtxCount,
                                  size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride, size_t
                                  TangentStride, size_t BiNormalStride, size_t TextureCoordsStride);

};

#define HEADER 0xFE55
#define FOOTER 0x55FE

#endif //_ENGINE_MESH_20161222
