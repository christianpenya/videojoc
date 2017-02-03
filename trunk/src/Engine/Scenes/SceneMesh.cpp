#include "SceneMesh.h"
#include "Mesh\Mesh.h"
#include "Mesh\MeshManager.h"
#include "Render\RenderManager.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "Engine\Engine.h"
#include "Utils\Name.h"
#include "Scenes\ConstantBufferManager.h"

CSceneMesh::CSceneMesh(CXMLElement* aElement)
    : CSceneNode(aElement)
    , mMesh( CEngine::GetInstance().GetMeshManager().GetMesh(aElement->GetAttribute<std::string>("mesh", "")) )
{
}

CSceneMesh::CSceneMesh(CXMLElement* aElement, CMesh* aMesh)
    : CSceneMesh(aElement)
{
    mMesh = aMesh;
}

CSceneMesh::~CSceneMesh() {}

bool CSceneMesh::Render(CRenderManager& aRendermanager)
{
    bool lOk = false;

    if (mMesh)
    {
        CConstantBufferManager& lCB = CEngine::GetInstance().GetConstantBufferManager();
        lCB.mObjDesc.m_World = GetMatrix();

        lCB.BindVSBuffer(aRendermanager.GetDeviceContext(), CConstantBufferManager::CB_Object);
        lOk = mMesh->Render(aRendermanager);
    }

    return lOk;
}