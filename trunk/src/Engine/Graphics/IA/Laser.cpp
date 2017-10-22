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
    , m_NormalSpeed(aTreeNode->GetAttribute<float>("normalSpeed", 15.0f))
    , m_SprintSpeed(aTreeNode->GetAttribute<float>("sprintSpeed", 25.0f))
    , m_Timer(aTreeNode->GetAttribute<float>("timer", 2.0f))
    , m_GunPosition(aTreeNode->GetAttribute<Vect3f>("gunPosition", Vect3f(0.0f, 0.0f, 0.0f)))
    , m_StartPosition(aTreeNode->GetAttribute<Vect3f>("startPosition", Vect3f(0.0f,0.0f,0.0f)))
    , m_EndPosition(aTreeNode->GetAttribute<Vect3f>("endPosition", Vect3f(0.0f, 0.0f, 0.0f)))
    , m_BoundMinX(aTreeNode->GetAttribute<float>("boundMinX", 0.0f))
    , m_BoundMaxX(aTreeNode->GetAttribute<float>("boundMaxX", 0.0f))
    , m_BoundMinZ(aTreeNode->GetAttribute<float>("boundMinZ", 0.0f))
    , m_BoundMaxZ(aTreeNode->GetAttribute<float>("boundMaxZ", 0.0f))
    , m_StartTime(aTreeNode->GetAttribute<float>("startTime", 0.0f))
    , m_Speed(0.0f)
    , m_Search(true)
{
    m_Timer = 2.0f;
    CRenderManager& lRM = CEngine::GetInstance().GetRenderManager();
    CMaterialManager& lMM = CEngine::GetInstance().GetMaterialManager();

    m_StartPosition = Vect3f(GetRandomValue(m_BoundMinX, m_BoundMaxX),0.0f, GetRandomValue(m_BoundMinZ, m_BoundMaxZ));
    mMaterials = lMM(aTreeNode->GetAttribute<std::string>("material", ""));
}

bool CLaser::Render(CRenderManager& lRM)
{
    bool lOk = true;
    if (m_Search)
    {
        mMaterials->Apply();
        CUSTOMVERTEX Vertices[] =
        {
            { m_GunPosition.x, m_GunPosition.y, m_GunPosition.z},
            { m_StartPosition.x, m_StartPosition.y, m_StartPosition.z}
        };

        CGeometry* lgeometry = new CGeometryLinesList<VertexTypes::Line>(new CVertexBuffer<VertexTypes::Line>(lRM, Vertices, 2));
        mGeometries = lgeometry;
        mGeometries->Render(lRM.GetDeviceContext());
    }
    return lOk;
}


bool CLaser::Update(float ElapsedTime)
{
    m_Timer -= ElapsedTime;

    if (m_StartPosition.Distance(m_EndPosition) <= 1.0f)
        CalculateNextPositionLaser(ElapsedTime);

    FireLaser();
    return true;
}


float CLaser::GetRandomValue(float min, float max)
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}


void CLaser::FireLaser()
{

    //(*this->GetParent())(m_patrolPoints[m_DestPoint].data())->GetPosition();

    //if (player.GetComponentInParent<PlayerControllerTPC>().sprint)
    //	m_Speed = m_SprintSpeed;
    //else
    m_Speed = m_NormalSpeed;
    m_StartPosition.Lerp(m_EndPosition, m_Speed);

    /*if (m_PhysXManager.GetActorPosition("player").y > -10.0f)
    {*/
    bool hitted = m_PhysXManager.Raycast(m_StartPosition, m_PhysXManager.GetActorPosition("player"), 0001, resultado);

    if (hitted && (resultado->actor == "player"))
    {
        if ((resultado->distance >= -m_DeadDistance) && (resultado->distance <= m_DeadDistance))
        {
            m_Search = false;
            std::cout << "Muere protagonista" << std::endl;
        }
    }
    //}
}

void CLaser::CalculateNextPositionLaser(float ElapsedTime)
{
    m_EndPosition = Vect3f(GetRandomValue(m_BoundMinX, m_BoundMaxX), m_PhysXManager.GetActorPosition("player").y, GetRandomValue(m_BoundMinZ, m_BoundMaxZ));
    m_StartTime = ElapsedTime;

    if (m_Timer < 0.1)
    {
        m_EndPosition = m_PhysXManager.GetActorPosition("player");
        m_Timer = 2.0f;
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