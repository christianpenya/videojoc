#pragma once
#include "Laser.h"
#include "Engine\engine.h"
#include "XML\xml.h"
#include "Physx\PhysxManager.h"
#include "Graphics\Mesh\TemplatedIndexedGeometry.h"
#include "Graphics\Mesh\VertexTypes.h"
#include "Graphics\Materials\MaterialManager.h"
#include "Math\MathUtils.h"
#include <iostream>


CLaser::~CLaser() {}

CLaser::CLaser(CXMLElement* aTreeNode)
    : CEnemy(aTreeNode)
    , CSceneNode(aTreeNode)
    , m_NormalSpeed(aTreeNode->GetAttribute<float>("normalSpeed", 0.5f))
    , m_SprintSpeed(aTreeNode->GetAttribute<float>("sprintSpeed", 0.8f))
    , m_Timer(aTreeNode->GetAttribute<float>("timer", 2.0f))
    , m_GunPosition(aTreeNode->GetAttribute<Vect3f>("gunPosition", Vect3f(0.0f, 0.0f, 0.0f)))
    , m_StartPosition(aTreeNode->GetAttribute<Vect3f>("startPosition", Vect3f(0.0f,0.0f,0.0f)))
    , m_EndPosition(aTreeNode->GetAttribute<Vect3f>("endPosition", Vect3f(0.0f, 0.0f, 0.0f)))
    , m_BoundMinX(aTreeNode->GetAttribute<float>("boundMinX", 0.0f))
    , m_BoundMaxX(aTreeNode->GetAttribute<float>("boundMaxX", 0.0f))
    , m_BoundMinZ(aTreeNode->GetAttribute<float>("boundMinZ", 0.0f))
    , m_BoundMaxZ(aTreeNode->GetAttribute<float>("boundMaxZ", 0.0f))
    , m_StartTime(aTreeNode->GetAttribute<float>("startTime", 0.0f))
    , m_Speed(m_NormalSpeed)
    , m_Search(true)
{
    m_Timer = 2.0f;
    CRenderManager& lRM = CEngine::GetInstance().GetRenderManager();
    CMaterialManager& lMM = CEngine::GetInstance().GetMaterialManager();

    m_StartPosition = Vect3f(GetRandomValue(m_BoundMinX, m_BoundMaxX),0.0f, GetRandomValue(m_BoundMinZ, m_BoundMaxZ));
    mMaterials = lMM(aTreeNode->GetAttribute<std::string>("material", ""));

    m_PhysXManager.AddCharacterController(GetName(), 1.1f, 0.17f, m_StartPosition, Quatf(), "Default", 0.5f, 0);

}

bool CLaser::Render(CRenderManager& lRM)
{
    bool lOk = true;
    mMaterials->Apply();
    CUSTOMVERTEX Vertices[] =
    {
        { m_GunPosition.x, m_GunPosition.y, m_GunPosition.z},
        { m_StartPosition.x, m_StartPosition.y, m_StartPosition.z}
    };

    CGeometry* lgeometry = new CGeometryLinesList<VertexTypes::Line>(new CVertexBuffer<VertexTypes::Line>(lRM, Vertices, 2));
    mGeometries = lgeometry;
    mGeometries->Render(lRM.GetDeviceContext());
    return lOk;
}


bool CLaser::Update(float ElapsedTime)
{
    m_Timer -= ElapsedTime;
    if (m_Search)
    {
        FireLaser();
        if (m_StartPosition.Distance(m_EndPosition) <= 0.02f)
            CalculateNextPositionLaser(ElapsedTime);
    }
    return true;
}




void CLaser::FireLaser()
{

    if (!CEngine::GetInstance().m_LevelController->GetTimePaused())
    {
        m_StartPosition.Lerp(m_EndPosition, m_Speed);
        CPhysXManager::RaycastData* resultado = new CPhysXManager::RaycastData();
        bool hittedP = m_PhysXManager.Raycast(m_GunPosition + (GetForward()*0.5f), m_StartPosition, 0, resultado); //funciona con grupo 0

        if (hittedP && (resultado->actor == "player"))
        {
            // m_Search = false;
            CEngine::GetInstance().m_LevelController->PlayerDetected();
            //std::cout << "Muere protagonista" << std::endl;
        }
        delete resultado;
    }
}

void CLaser::CalculateNextPositionLaser(float ElapsedTime)
{
    m_EndPosition = Vect3f(GetRandomValue(m_BoundMinX, m_BoundMaxX), m_PhysXManager.GetActorPosition("player").y + 1.3f, GetRandomValue(m_BoundMinZ, m_BoundMaxZ));
    m_StartTime = ElapsedTime;
    m_Speed = GetRandomValue(m_NormalSpeed, m_SprintSpeed);

    if (m_Timer < 0.1)
    {
        m_Speed = m_SprintSpeed;
        // m_EndPosition = m_PhysXManager.GetActorPosition("player") + Vect3f(0.0f, 1.3f, 0.0f);
        m_Timer = 1.5f;
    }

}



void CLaser::DrawImgui()
{
    if (ImGui::TreeNode(m_Name.c_str()))
    {
        mMaterials->DrawImgui();
        ImGui::TreePop();
    }
}