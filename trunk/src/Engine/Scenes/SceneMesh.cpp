#include "SceneMesh.h"
#include "Render\RenderManager.h"
#include "Engine.h"

bool CSceneMesh::Render(CRenderManager& aRendermanager)
{
    bool lOk = false;

    if (mMesh)
    {
        // TODO
        //CConstantBufferManager& lCB = CEngine::GetInstance().GetConstantBufferManager();
        //lCB.mObjDesc.m_World = GetMatrix();
        //lCB.BindVSBuffer(aRendermanager.GetDeviceContext(), CConstantBufferManager::CB_Object);
        //lOk = mMesh->Render(aRendermanager);
    }

    return lOk;
}