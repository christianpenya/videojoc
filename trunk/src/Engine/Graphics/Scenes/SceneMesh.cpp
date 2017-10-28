#include "SceneMesh.h"
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/MeshManager.h"
#include "Render\RenderManager.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "Engine\Engine.h"
#include "Graphics/Buffers/ConstantBufferManager.h"
#include "Utils\EnumToString.h"
#include "Utils/Logger.h"
#include "Math/Quaternion.h"

CSceneMesh::CSceneMesh(CXMLElement* aElement)
    : CSceneNode(aElement)
    , mMesh(CEngine::GetInstance().GetMeshManager().GetMesh(aElement->GetAttribute<std::string>("mesh", "")))
    , mRigidBodyEnum(eRigidBodyCount)
    , mPhysxIndex(0)
{
    m_ignoreFrustum = aElement->GetAttribute<bool>("ignore_frustum", false);
    m_NodeType = CSceneNode::eMesh;
    //COLLIDERS

    for (tinyxml2::XMLElement *iCollider = aElement->FirstChildElement(); iCollider != nullptr; iCollider = iCollider->NextSiblingElement())
    {
        if (strcmp(iCollider->Name(), "collider") == 0)
        {
            // (strcmp(aElement->FirstChildElement()->Name(), "transform") == 0) ? aElement->FirstChildElement() : nullptr;
            std::string lRigidBody = iCollider->GetAttribute<std::string>("rigid_body", "");
            std::string lFilename = iCollider->GetAttribute<std::string>("filename", "");
            int lGroup = iCollider->GetAttribute<int>("group", 3);
            Vect3f lOffset = iCollider->GetAttribute<Vect3f>("offset", Vect3f(0.0f, 0.0f, 0.0f));
            float lYawOffset = mathUtils::Deg2Rad(iCollider->GetAttribute<float>("yaw", 0.0f));
            float lPitchOffset = mathUtils::Deg2Rad(iCollider->GetAttribute<float>("pitch", 0.0f));
            float lRollOffset = mathUtils::Deg2Rad(iCollider->GetAttribute<float>("roll", 0.0f));
            m_Name = iCollider->GetAttribute<std::string>("name", m_Name);

            bool isKinematic = iCollider->GetAttribute<bool>("kinematic", false);
            Vect3f lColliderPosition;

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

                sizeX = abs(abs(lAABB.GetMax().x - lAABB.GetMin().x) * m_Scale.x);
                sizeY = abs(abs(lAABB.GetMax().y - lAABB.GetMin().y) * m_Scale.y);
                sizeZ = abs(abs(lAABB.GetMax().z - lAABB.GetMin().z) * m_Scale.z);
                cubeOffset = Vect3f(mMesh->GetBoundingSphere().GetCenter().x, -mMesh->GetBoundingSphere().GetCenter().y, 0);

                lCenter = m_Position + lOffset;
                mPhysxIndex = CEngine::GetInstance().GetPhysXManager().CreateStaticBox(m_Name, "Default", rotation, lCenter, sizeX, sizeY, sizeZ);

                //rotation = CEngine::GetInstance().GetPhysXManager().GetActorOrientation(m_Name);
                //m_Position = CEngine::GetInstance().GetPhysXManager().GetActorPosition(m_Name) + rotation.Rotate(cubeOffset);
                //m_Pitch = rotation.GetRotationMatrix().GetAngleX();
                //m_Yaw = rotation.GetRotationMatrix().GetAngleY();
                //m_Roll = rotation.GetRotationMatrix().GetAngleZ();
                break;

            case eDynamicBox:
            case eEnemy:
                lDebug = "Attached physx DYNAMIC BOX to " + m_Name;
                rotation.QuatFromYawPitchRoll(m_Yaw, m_Pitch, m_Roll);

                sizeX = abs(abs(lAABB.GetMax().x - lAABB.GetMin().x) * m_Scale.x);
                sizeY = abs(abs(lAABB.GetMax().y - lAABB.GetMin().y) * m_Scale.y);
                sizeZ = abs(abs(lAABB.GetMax().z - lAABB.GetMin().z) * m_Scale.z);
                cubeOffset = Vect3f(mMesh->GetBoundingSphere().GetCenter().x, -mMesh->GetBoundingSphere().GetCenter().y, 0);

                lCenter = m_Position;
                CEngine::GetInstance().GetPhysXManager().CreateDynamicBox(m_Name, "Default", rotation, lCenter, sizeX, sizeY, sizeZ, 0.5f, isKinematic, lGroup);

                rotation = CEngine::GetInstance().GetPhysXManager().GetActorOrientation(m_Name);
                m_Position = CEngine::GetInstance().GetPhysXManager().GetActorPosition(m_Name) + rotation.Rotate(cubeOffset);
                m_Pitch = rotation.GetRotationMatrix().GetAngleX();
                m_Yaw = rotation.GetRotationMatrix().GetAngleY();
                m_Roll = rotation.GetRotationMatrix().GetAngleZ();

                break;

            case eShape:
                lDebug = "Attached physx SHAPE to " + m_Name;
                rotation.QuatFromYawPitchRoll(m_Yaw + lYawOffset, m_Pitch + lPitchOffset, m_Roll + lRollOffset);

                assert(strcmp(lFilename.c_str(), "") != 0);
                lColliderPosition = m_Position + lOffset;

                CEngine::GetInstance().GetPhysXManager().CreateStaticShape(m_Name, "Default", rotation, lColliderPosition, lFilename, lGroup);

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

                sizeX = abs(abs(lAABB.GetMax().x - lAABB.GetMin().x) * m_Scale.x);
                sizeY = abs(abs(lAABB.GetMax().y - lAABB.GetMin().y) * m_Scale.y);
                sizeZ = abs(abs(lAABB.GetMax().z - lAABB.GetMin().z) * m_Scale.z);
                cubeOffset = Vect3f(mMesh->GetBoundingSphere().GetCenter().x, -mMesh->GetBoundingSphere().GetCenter().y, 0);
                lCenter = m_Position + mMesh->GetBoundingSphere().GetCenter();

                CEngine::GetInstance().GetPhysXManager().AddTriggerBox(m_Name, sizeX, sizeY, sizeZ, lCenter, rotation);
                break;

            default:
                lDebug = "NO physx were added to " + m_Name;
                break;
            }
        }
    }
}

void CSceneMesh::Initialize(CXMLElement* aElement)
{
    mMesh = CEngine::GetInstance().GetMeshManager().GetMesh(aElement->GetAttribute<std::string>("mesh", ""));

    m_ignoreFrustum = aElement->GetAttribute<bool>("ignore_frustum", false);
    m_NodeType = CSceneNode::eMesh;
    //COLLIDERS

    for (tinyxml2::XMLElement *iNode = aElement->FirstChildElement(); iNode != nullptr; iNode = iNode->NextSiblingElement())
    {
        // BEAR IN MIND, for proper update transform tag must be read before collider
        if (strcmp(iNode->Name(), "transform") == 0)
        {
            m_Position = iNode->GetAttribute<Vect3f>("position", Vect3f(0.0f, 0.0f, 0.0f));
            m_PrevPos = m_Position;
            m_Scale = iNode->GetAttribute<Vect3f>("scale", Vect3f(1.0f, 1.0f, 1.0f));
            m_Yaw = mathUtils::Deg2Rad(iNode->GetAttribute<float>("yaw", 0.0f));
            m_Pitch = mathUtils::Deg2Rad(iNode->GetAttribute<float>("pitch", 0.0f));
            m_Roll = mathUtils::Deg2Rad(iNode->GetAttribute<float>("roll", 0.0f));
        }
        else if (strcmp(iNode->Name(), "collider") == 0)
        {
            Quatf rotation = Quatf();
            rotation.QuatFromYawPitchRoll(m_Yaw, m_Pitch, m_Roll);
            CEngine::GetInstance().GetPhysXManager().SetActorTransform(m_Name, m_Position, rotation);
        }
    }
}

CSceneMesh::CSceneMesh(CXMLElement* aElement, CMesh* aMesh)
    : CSceneMesh(aElement)

{
    mMesh = aMesh;
    mPhysxIndex = 0;

    //Reescalado del radio de la bounding sphere según escena
    float radius = mMesh->GetBoundingSphere().GetRadius();
    mMesh->GetBoundingSphere().SetRadius(radius * m_Scale.x);
}

CSceneMesh::~CSceneMesh()
{
    __H_CHECKED_DELETE__(mMesh);
}

bool CSceneMesh::Update(float aDeltaTime)
{
    bool lOk = true;

    if (m_Active)
    {
        mBS = mMesh->GetBoundingSphere();

        if (mRigidBodyEnum < eRigidBodyCount)
        {
            CPhysXManager& lPM = CEngine::GetInstance().GetPhysXManager();
            Quatf lRotation;

            switch (mRigidBodyEnum)
            {
            case ePlane:
                break;
            case eSphere:
            case eBox:

                break;
            case eShape:
                break;
            case ePlayer:
                m_Position = lPM.GetActorPosition(m_Name);
                break;

            case eDynamicBox:
            case eEnemy:
                m_Position = lPM.GetActorPosition(m_Name);
                break;

            case eTriggerBox:
                break;
            default:
                break;
            }
        }
    }

    return lOk;
}

void CSceneMesh::DeletePhysx()
{
    CEngine::GetInstance().GetPhysXManager().DeleteActor(m_Name, mPhysxIndex);
}

void CSceneMesh::Deactivate()
{
    DeletePhysx();
    m_Active = false;
}

bool CSceneMesh::Render(CRenderManager& aRendermanager)
{
    bool lOk = true;

    if (m_Active)
    {
        lOk = CSceneNode::Render(aRendermanager);

        if (lOk && mMesh)
        {
            CConstantBufferManager& lCB = CEngine::GetInstance().GetConstantBufferManager();
            lCB.mObjDesc.m_World = GetMatrix();

            lCB.BindBuffer(aRendermanager.GetDeviceContext(), CConstantBufferManager::CB_Object);
            lOk = mMesh->Render(aRendermanager);
        }
        else
            lOk = false;
    }

    return lOk;
}

bool CSceneMesh::HasGotPhysx()
{
    bool out = true;

    if (mPhysxIndex == 0)
    {
        out = false;
    }

    return out;
}

void CSceneMesh::DrawImgui()
{
    if (ImGui::TreeNode(m_Name.c_str()))
    {
        ImGui::SliderFloat("XPosition", (float*)&m_Position.x, mOriginalUnmodifiedPosition.x - 1.0f, mOriginalUnmodifiedPosition.x + 1.0f);
        ImGui::SliderFloat("YPosition", (float*)&m_Position.y, mOriginalUnmodifiedPosition.y - 1.0f, mOriginalUnmodifiedPosition.y + 1.0f);
        ImGui::SliderFloat("ZPosition", (float*)&m_Position.z, mOriginalUnmodifiedPosition.z - 1.0f, mOriginalUnmodifiedPosition.z + 1.0f);

        ImGui::SliderFloat3("Scale", (float*)&m_Scale, -2.0f, 2.0f);

        float lYawTmp = mathUtils::Rad2Deg(m_Yaw);
        float lPitchTmp = mathUtils::Rad2Deg(m_Pitch);
        float lRollTmp = mathUtils::Rad2Deg(m_Roll);

        ImGui::SliderFloat("Yaw", (float*)&lYawTmp, -180.0f, 180.0f);
        ImGui::SliderFloat("Pitch", (float*)&lPitchTmp, -180.0f, 180.0f);
        ImGui::SliderFloat("Roll", (float*)&lRollTmp, -180.0f, 180.0f);

        m_Yaw = mathUtils::Deg2Rad(lYawTmp);
        m_Pitch = mathUtils::Deg2Rad(lPitchTmp);
        m_Roll = mathUtils::Deg2Rad(lRollTmp);
        mMesh->DrawImGui();
        ImGui::TreePop();
    }

}
