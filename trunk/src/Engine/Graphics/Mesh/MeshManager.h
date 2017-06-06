#pragma once
#ifndef _ENGINE_MESHMANAGER_20161222
#define _ENGINE_MESHMANAGER_20161222

#include "Utils\TemplatedMapVector.h"
#include "Mesh.h"

class CMesh;
class CMeshManager
{
public:
    CMeshManager();
    virtual ~CMeshManager();
    CMesh* GetMesh(const std::string& aFilename);
    void Destroy();

private:
    base::utils::CTemplatedMapVector<CMesh> mMeshes;
    bool Load(const std::string& aFilename);
};

#endif //_ENGINE_MESHMANAGER_20161222
