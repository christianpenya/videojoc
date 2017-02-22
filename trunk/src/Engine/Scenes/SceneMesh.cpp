#include "SceneMesh.h"
#include "Mesh\Mesh.h"
#include "Mesh\MeshManager.h"
#include "Render\RenderManager.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "Engine\Engine.h"
#include "Scenes\ConstantBufferManager.h"

CSceneMesh::CSceneMesh(CXMLElement* aElement)
    : CSceneNode(aElement)
    , mMesh( CEngine::GetInstance().GetMeshManager().GetMesh(aElement->GetAttribute<std::string>("mesh", "")) )
{
    m_type = 0;
}

CSceneMesh::CSceneMesh(CXMLElement* aElement, CMesh* aMesh)
    : CSceneMesh(aElement)
{
    mMesh = aMesh;
}

CSceneMesh::~CSceneMesh() {}

bool CSceneMesh::Update(float aDeltaTime)
{
    bool lOk = true;

    mBS = mMesh->GetBoundingSphere();

    return lOk;
}

bool CSceneMesh::Render(CRenderManager& aRendermanager)
{
    bool lOk = CSceneNode::Render(aRendermanager);

    if (lOk && mMesh)
    {
        CConstantBufferManager& lCB = CEngine::GetInstance().GetConstantBufferManager();
        lCB.mObjDesc.m_World = GetMatrix();

        lCB.BindBuffer(aRendermanager.GetDeviceContext(), CConstantBufferManager::CB_Object);
        lOk = mMesh->Render(aRendermanager);
    }

    return lOk;
}

void CSceneMesh::DrawImgui()
{
    if (ImGui::CollapsingHeader(m_Name.c_str()))
    {
        ImGui::SliderFloat3("Position", (float*)&m_Position, -100.0f, 100.0f);
        ImGui::SliderFloat3("Scale", (float*)&m_Scale, 0.0f, 100.0f);
        //ImGui::Checkbox("Visible", &m_Visible);
    }
}
