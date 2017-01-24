#pragma once
#ifndef _ENGINE_MESHMANAGER_20161222
#define _ENGINE_MESHMANAGER_20161222

#include <string>
#include "Utils\TemplatedMapVector.h"
#include "Mesh.h"

class CMesh;
class CMeshManager
{
public:
    CMeshManager();
    virtual ~CMeshManager();
    CMesh* GetMesh(const std::string& aFilename);
private:
    base::utils::CTemplatedMapVector<CMesh> mMeshes;
    bool Load(const std::string& aFilename);
    void Destroy();
};

#endif //_ENGINE_MESHMANAGER_20161222
