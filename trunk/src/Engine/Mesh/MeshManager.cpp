#include "MeshManager.h"
#include "Mesh.h"

CMesh* CMeshManager::GetMesh(const std::string& aFilename) {

    if (!mMeshes.Exist(aFilename)) {
        Load(aFilename);
    }

    CMesh* lMesh = mMeshes(aFilename);

    return lMesh;
}

bool CMeshManager::Load(const std::string& aFilename)
{
    bool lOk = false;
    CMesh * lMesh = new CMesh();
    if (lMesh->Load(aFilename))
    {
        lOk = mMeshes.Add(aFilename, lMesh);
    }
    else
    {
        base::utils::CheckedDelete(lMesh);
    }

    return lOk;
}

void CMeshManager::Destroy() {
    mMeshes.Destroy();
}