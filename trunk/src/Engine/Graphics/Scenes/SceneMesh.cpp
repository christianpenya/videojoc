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
    , mOriginalUnmodifiedPosition(m_Position)
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

                lCenter = m_Position;// +mMesh->GetBoundingSphere().GetCenter();
                mPhysxIndex = CEngine::GetInstance().GetPhysXManager().CreateStaticBox(m_Name, "Default", rotation, lCenter, sizeX, sizeY, sizeZ);

                rotation = CEngine::GetInstance().GetPhysXManager().GetActorOrientation(m_Name);
                m_Position = CEngine::GetInstance().GetPhysXManager().GetActorPosition(m_Name) + rotation.Rotate(cubeOffset);
                m_Pitch = rotation.GetRotationMatrix().GetAngleX();
                m_Yaw = rotation.GetRotationMatrix().GetAngleY();
                m_Roll = rotation.GetRotationMatrix().GetAngleZ();
                break;

            case eDynamicBox:
            case eEnemy:
                lDebug = "Attached physx ENEMY CONTROLLER to " + m_Name;
                rotation.QuatFromYawPitchRoll(m_Yaw, m_Pitch, m_Roll);

                sizeX = abs(lAABB.GetMax().x - lAABB.GetMin().x) * m_Scale.x;
                sizeY = abs(lAABB.GetMax().y - lAABB.GetMin().y) * m_Scale.y;
                sizeZ = abs(lAABB.GetMax().z - lAABB.GetMin().z) * m_Scale.z;
                cubeOffset = Vect3f(0, -sizeY / 2.0, 0);

                lCenter = mMesh->GetBoundingSphere().GetCenter() + m_Position;
                CEngine::GetInstance().GetPhysXManager().CreateDynamicBox(m_Name, "Default", rotation, lCenter, sizeX, sizeY, sizeZ, 0.5f);
                break;

            case eShape:
                lDebug = "Attached physx SHAPE to " + m_Name;
                rotation.QuatFromYawPitchRoll(m_Yaw, m_Pitch, m_Roll);

                assert(strcmp(lFilename.c_str(), "") != 0);

                CEngine::GetInstance().GetPhysXManager().CreateStaticShape(m_Name, "Default", rotation, m_Position, lFilename);
                //CEngine::GetInstance().GetPhysXManager().CreateStaticTriangleMesh(m_Name, "Default", rotation, m_Position, );
                // CEngine::GetInstance().GetPhysXManager().CreateStaticShape(m_Name, "Default", rotation, m_Position, );
                // mMesh->;
                // CEngine::GetInstance().GetPhysXManager().CreateDynamicShape(m_Name, "Default", rotation, m_Position, nullptr, 1.0f);
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

            LOG_INFO_APPLICATION(lDebug.c_str());
        }
    }
}

CSceneMesh::CSceneMesh(CXMLElement* aElement, CMesh* aMesh)
    : CSceneMesh(aElement)
{
    mMesh = aMesh;

    //Reescalado del radio de la bounding sphere según escena
    float radius = mMesh->GetBoundingSphere().GetRadius();
    mMesh->GetBoundingSphere().SetRadius(radius * m_Scale.x);
}

CSceneMesh::~CSceneMesh() {}

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

void CSceneMesh::DrawImgui()
{
    if (ImGui::CollapsingHeader(m_Name.c_str()))
    {
        ImGui::SliderFloat("XPosition", (float*)&m_Position.x, mOriginalUnmodifiedPosition.x - 5.0f, mOriginalUnmodifiedPosition.x + 5.0f);
        ImGui::SliderFloat("YPosition", (float*)&m_Position.y, mOriginalUnmodifiedPosition.y - 5.0f, mOriginalUnmodifiedPosition.y + 5.0f);
        ImGui::SliderFloat("ZPosition", (float*)&m_Position.z, mOriginalUnmodifiedPosition.z - 5.0f, mOriginalUnmodifiedPosition.z + 5.0f);

        ImGui::SliderFloat3("Scale", (float*)&m_Scale, 0.0f, 5.0f);

        float lYawTmp = mathUtils::Rad2Deg(m_Yaw);
        float lPitchTmp = mathUtils::Rad2Deg(m_Pitch);
        float lRollTmp = mathUtils::Rad2Deg(m_Roll);

        ImGui::SliderFloat("Yaw", (float*)&lYawTmp, -180.0f, 180.0f);
        ImGui::SliderFloat("Pitch", (float*)&lPitchTmp, -180.0f, 180.0f);
        ImGui::SliderFloat("Roll", (float*)&lRollTmp, -180.0f, 180.0f);

        m_Yaw = mathUtils::Deg2Rad(lYawTmp);
        m_Pitch = mathUtils::Deg2Rad(lPitchTmp);
        m_Roll = mathUtils::Deg2Rad(lRollTmp);
    }
}
