#include "Graphics\IA\Guard.h"
#include "Graphics\Mesh\MeshManager.h"
#include "Physx\PhysxManager.h"
#include "Engine\engine.h"
#include "Utils\Defines.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "Utils\EnumToString.h"
#include "Utils\Logger.h"
#include "XML\XML.h"
#include "Pathfinding.h"

CGuard::~CGuard()
{

}

CGuard::CGuard(CXMLElement* aTreeNode)
    : CEnemyAnimated(aTreeNode)
    , m_DetectAngle(aTreeNode->GetAttribute<float>("detectAngle", 45.0f))
    , m_soundDetected(aTreeNode->GetAttribute<std::string>("soundDetected", ""))
    , m_speedPatroling(aTreeNode->GetAttribute<float>("speedPatroling", 0.8f))
    , m_speedChasing(aTreeNode->GetAttribute<float>("speedChasing", 1.0f))
    , m_investigatingTolerance(aTreeNode->GetAttribute<float>("investigatingTolerance", 1.0f))
    , m_lastPositionView(Vect3f(0.0f, 0.0f, 0.0f))
{
    const tinyxml2::XMLElement* aElement = aTreeNode->FirstChildElement();

    while (aElement != NULL)
    {
        if (strcmp(aElement->Name(), "point") == 0)
            m_patrolPoints.push_back(aElement->GetAttribute<std::string>("name", ""));
        aElement = aElement->NextSiblingElement();
    }
    m_accumtime = 0.0f;
    m_sleep = false;
    m_timeSleep = GetRandomValue(0.0f, 2.0f);

}


void CGuard::DrawImgui()
{
    if (ImGui::TreeNode(m_Name.c_str()))
    {
        ImGui::Checkbox("Visible", &m_Visible);
        ImGui::SliderFloat("Sight Distance", &m_SightDistance, 0.0f, 20.0f, "%.2f");
        ImGui::SliderFloat("Max Detect Distance", &m_MaxDetectDistance, 0.0f, 20.0f, "%.2f");
        ImGui::SliderFloat("Dead Distance", &m_DeadDistance, 0.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("Detect Angle", &m_DetectAngle, 0.0f, 360.0f, "%.2f");
        ImGui::SliderFloat("Speed Patroling", &m_speedPatroling, 0.01f, 20.0f, "%.2f");
        ImGui::SliderFloat("Speed Chasing", &m_speedChasing, 0.01f, 20.0f, "%.2f");
        ImGui::SliderFloat("Investigating Tolerance", &m_investigatingTolerance, 0.01f, 20.0f, "%.2f");
        ImGui::SliderFloat3("Position", (float*)&m_Position, -500.0f, 500.0f, "%.2f");
        ImGui::SliderFloat3("Scale", (float*)&m_Scale, 0.0f, 100.0f, "%.2f");
        ImGui::TreePop();
    }
}


void CGuard::dieEnemy(Vect3f pos)
{
    m_enemydead = true;
    ClearActiveAnimationCycle(0.5f);
    ExecuteAction(2, 0.5f, 2.0f, 1.0f, true);
}


void CGuard::patrol()
{
    hearsPlayer();


    if (m_standby)
    {
        ClearActiveAnimationCycle(0.5f);
        BlendCycle(0, 0.5f, 0.5f);
        m_DestPoint = 1;
        this->SetForward(GetPatrolPosition() - m_Position);

    }
    else
    {
        Sleep();
        if (!m_sleep)
        {
            //    std::cout << " Guardia " << GetName() << "patroling." << std::endl;
            if ((m_Destination == Vect3f{ 0.0f, -99.9f, 0.0f }) || (distanceBetweenTwoPoints(m_Position.x, m_Position.z, m_Destination.x, m_Destination.z) <= 1.0f))
                GotoNextPoint();

            float l_speed = GetRandomValue(0.0f, m_speedPatroling);
            Move(m_Destination, l_speed);
        }
    }

}

void CGuard::Sleep()
{
    m_accumtime += m_ElapsedTime;
    if (m_accumtime > m_timeSleep)
    {
        m_accumtime = 0;
        m_sleep = !m_sleep;
        if (m_sleep)
        {
            //std::cout << " Guardia " << GetName() << " sleep." << std::endl;
            ClearActiveAnimationCycle(0.5f);
            BlendCycle(0, 0.5f, 0.5f);
        }
        else
        {
            //std::cout << " Guardia " << GetName() << " caminando." << std::endl;
            ClearActiveAnimationCycle(0.5f);
            BlendCycle(1, 0.5f, 0.5f);
            m_timeSleep = GetRandomValue(0.0f, 10.0f);

        }
    }

}

void CGuard::GotoNextPoint()
{


    if (m_pathChasingAux.size() > 0)  //Si se salió de la ruta para perseguir, se calcula de nuevo el camino de regreso
    {
        m_Destination = m_pathChasingAux[m_pathChasingAux.size() - 1];
        //m_Destination = Vect3f(m_Destination.x, m_Height.y, m_Destination.z);
        m_pathChasingAux.pop_back();
    }
    else
    {
        if (m_patrolPoints.size() == 0)
            return;

        m_Destination = GetPatrolPosition();
        m_DestPoint = (m_DestPoint + 1) % m_patrolPoints.size();
        //m_Destination = Vect3f(m_Destination.x, m_Height.y, m_Destination.z);
    }
}

void CGuard::chase()
{
    //  std::cout << " Guardia " << GetName() << "chasing." << std::endl;

    if ((!m_Calculated) || (distanceBetweenTwoPoints(m_Position.x, m_Position.z, m_DestinationChase.x, m_DestinationChase.z) <= 0.2f))
        GotoNextPointChase();

    float l_speed =  GetRandomValue(m_speedPatroling, m_speedChasing);
    Move(m_DestinationChase, l_speed);
}

void CGuard::Move(Vect3f destination, float speed)
{
    ClearActiveAnimationCycle(0.5f);
    BlendCycle(1, 1, 0.5f);

    Vect3f lastMove = m_Movement;
    m_Movement.Lerp(destination, speed);
    m_Position = m_Movement;
    this->SetForward(destination - m_Position);
    m_PhysXManager.MoveCharacterController(GetName(), m_Position - lastMove, PHYSX_UPDATE_STEP);
    this->SetPosition(m_PhysXManager.GetActorPosition(GetName()) + m_Height);

    //std::cout << " Guardia " << GetName() << " yaw " << GetYaw() << " pitch " << GetPitch() <<" roll " << GetRoll() << std::endl;

}

void CGuard::GotoNextPointChase()
{
    m_lastPositionView = m_PhysXManager.GetActorPosition("player");

    CPhysXManager::RaycastData* result = new CPhysXManager::RaycastData();
    bool hitted = m_PhysXManager.Raycast(m_PhysXManager.GetActorPosition("player") + Vect3f(0.0f, 1.0f, 0.0f), m_Position + Vect3f(0.0f, 1.0f, 0.0f), m_Group, result); //Funciona con grupo 0


    if (hitted && (result->actor == "player"))
        m_DestinationChase = m_lastPositionView;
    else
    {
        if (!m_Calculated)
            ComputePath();

        if (m_ppath->PathfindStep())
        {
            m_pathChasing = m_ppath->GetPath();
            if (m_pathChasing.size() == 0)
                return;

            if ((m_DestPointChase + 1) == m_pathChasing.size())
                ComputePath();

            m_DestPointChase = (m_DestPointChase + 1) % m_pathChasing.size();
            m_DestinationChase = m_pathChasing[m_DestPointChase];
            //m_DestinationChase = Vect3f(m_DestinationChase.x, m_Height.y, m_DestinationChase.z);
        }
        else
            ComputePath();
    }
    delete result;
}


void CGuard::ComputePath()
{
    m_lastPositionView = m_PhysXManager.GetActorPosition("player");

    //std::cout << " Guardia " << GetName() << "chasing, calculó nuevo path" << std::endl;
    m_ppath = new CPathfinding(m_lastPositionView, m_Position, m_pnavMesh->GetName());
    m_DestPointChase = 0;
    m_Calculated = true;

    if (m_ppath->PathfindStep())
        m_pathChasing.push_back(m_lastPositionView);
}