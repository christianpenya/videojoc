#include "SceneMesh.h"
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/MeshManager.h"
#include "Render\RenderManager.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "Engine\Engine.h"
#include "Graphics/Buffers/ConstantBufferManager.h"
#include "Physx/PhysxManager.h"
#include "Utils\EnumToString.h"
#include "Utils/Logger.h"
#include "Math/Quaternion.h"

CSceneMesh::CSceneMesh(CXMLElement* aElement)
    : CSceneNode(aElement)
    , mMesh(CEngine::GetInstance().GetMeshManager().GetMesh(aElement->GetAttribute<std::string>("mesh", "")))
    , mRigidBodyEnum(eRigidBodyCount)
{
    m_ignoreFrustum = aElement->GetAttribute<bool>("ignore_frustum", false);
    m_NodeType = CSceneNode::eMesh;

    std::string lRigidBody = aElement->GetAttribute<std::string>("rigid_body", "");
    EnumString<ERigidBody>::ToEnum(mRigidBodyEnum, lRigidBody);

    CAxisAlignedBoundingBox lAABB = mMesh->GetAABB();
    float size = 1.0f;
    float sizeX, sizeY, sizeZ;
    Quatf rotation = Quatf();
    std::string lDebug;
    Vect3f lCenter;

    switch (mRigidBodyEnum)
    {
    case ePlane:
        lDebug = "Attached physx PLANE to " + m_Name;
        CEngine::GetInstance().GetPhysXManager().CreatePlane("Default", 0, 1, 0, 0, 1);
        break;

    case eBox:
        lDebug = "Attached physx BOX to " + m_Name;
        rotation.QuatFromYawPitchRoll(m_Yaw, m_Pitch, m_Roll);

        sizeX = abs(lAABB.GetMax().x - lAABB.GetMin().x) * m_Scale.x;
        sizeY = abs(lAABB.GetMax().y - lAABB.GetMin().y) * m_Scale.y;
        sizeZ = abs(lAABB.GetMax().z - lAABB.GetMin().z) * m_Scale.z;
        cubeOffset = Vect3f(0, -sizeY/2.0, 0);

        lCenter = mMesh->GetBoundingSphere().GetCenter() + m_Position;
        CEngine::GetInstance().GetPhysXManager().CreateDynamicBox(m_Name, "Default", rotation, lCenter, sizeX, sizeY, sizeZ, 0.5f);
        break;

    case eSphere:
        lDebug = "Attached physx SPHERE to " + m_Name;
        rotation.QuatFromYawPitchRoll(m_Yaw, m_Pitch, m_Roll);
        CEngine::GetInstance().GetPhysXManager().CreateDynamicSphere(m_Name, "Default", rotation, m_Position, size / 5, 0.5f);
        break;

    case ePlayer:
        lDebug = "Attached physx PLAYER CONTROLLER to " + m_Name;
        break;

    case eTriggerBox:
        lDebug = "Attached physx Trigger Box to " + m_Name;
        rotation.QuatFromYawPitchRoll(m_Yaw, m_Pitch, m_Roll);
        CEngine::GetInstance().GetPhysXManager().AddTriggerBox(m_Name, size, size, size, m_Position, rotation);

        break;
    default:
        lDebug = "NO physx were added to " + m_Name;
        break;
    }

    LOG_INFO_APPLICATION(lDebug.c_str());
}

CSceneMesh::CSceneMesh(CXMLElement* aElement, CMesh* aMesh)
    : CSceneMesh(aElement)
{
    mMesh = aMesh;
    float radius = mMesh->GetBoundingSphere().GetRadius();
    mMesh->GetBoundingSphere().SetRadius(radius * m_Scale.x); // TODO el reescalado es muy trapero
}

CSceneMesh::~CSceneMesh() {}

bool CSceneMesh::Update(float aDeltaTime)
{
    bool lOk = true;

    mBS = mMesh->GetBoundingSphere();

    if (mRigidBodyEnum < eRigidBodyCount)
    {
        CPhysXManager& lPM = CEngine::GetInstance().GetPhysXManager();
        Quatf lRotation;

        Vect3f hola;
        switch (mRigidBodyEnum)
        {
        case ePlane:
            break;
        case eSphere:
        case eBox:

            lRotation = lPM.GetActorOrientation(m_Name);
            m_Position = lPM.GetActorPosition(m_Name) + lRotation.Rotate(cubeOffset);
            m_Pitch = lRotation.GetRotationMatrix().GetAngleX();
            m_Yaw = lRotation.GetRotationMatrix().GetAngleY();
            m_Roll = lRotation.GetRotationMatrix().GetAngleZ();

            lRotation.GetRotationMatrix().GetYaw();
            break;
        case ePlayer:
            m_Position = lPM.GetActorPosition(m_Name);
            break;
        default:
            break;
        }
    }

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
    else
        lOk = false;

    return lOk;
}

void CSceneMesh::DrawImgui()
{
    if (ImGui::CollapsingHeader(m_Name.c_str(), ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::SliderFloat3("Position", (float*)&m_Position, -100.0f, 100.0f);
        ImGui::SliderFloat3("Scale", (float*)&m_Scale, 0.0f, 100.0f);
        ImGui::Checkbox("Visible", &m_Visible);
        mMesh->DrawImGui();
    }
}
