#include "Graphics\IA\Dron.h"
#include "Graphics\Mesh\MeshManager.h"
#include "Physx\PhysxManager.h"
#include "Engine\engine.h"
#include "Utils\Defines.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "Utils\EnumToString.h"
#include "Utils\Logger.h"
#include "XML\XML.h"
#include "Pathfinding.h"

CDron::~CDron()
{

}

CDron::CDron(CXMLElement* aTreeNode)
    : CEnemy(aTreeNode, CEnemy::eDron)
    , m_DetectAngle(aTreeNode->GetAttribute<float>("detectAngle", 45.0f))
    , m_soundDetected(aTreeNode->GetAttribute<std::string>("soundDetected", ""))
    , m_speedPatroling(aTreeNode->GetAttribute<float>("speedPatroling", 0.8f))
    , m_speedChasing(aTreeNode->GetAttribute<float>("speedChasing", 1.0f))
    , m_investigatingTolerance(aTreeNode->GetAttribute<float>("investigatingTolerance", 1.0f))
    , m_lastPositionView(Vect3f(0.0f,0.0f,0.0f))
{
    const tinyxml2::XMLElement* aElement = aTreeNode->FirstChildElement();

    while (aElement != NULL)
    {
        if (strcmp(aElement->Name(), "point") == 0)
            m_patrolPoints.push_back(aElement->GetAttribute<std::string>("name", ""));
        aElement = aElement->NextSiblingElement();
    }

    m_PhysXManager.AddCharacterController(GetName(), this->GetScale().y, 0.25f, Vect3f(0, 0, 0), Quatf(0, 0, 0, 1), "Default", 0.5f);
}


void CDron::DrawImgui()
{
    if (ImGui::CollapsingHeader(m_Name.c_str()))
    {
        ImGui::SliderFloat("Sight Distance", &m_SightDistance, 0.01f, 20.0f, "%.2f", 0.01f);
        ImGui::SliderFloat("Max. Detect Distance", &m_MaxDetectDistance, 0.01f, 20.0f,"%.2f",0.01f);
        ImGui::SliderFloat("Dead Distance", &m_DeadDistance, 0.01f, 20.0f, "%.2f", 0.01f);
        ImGui::SliderFloat("Detect Angle", &m_DetectAngle, 0.0f, 360.0f, "%.2f", 0.01f);
        ImGui::SliderFloat("Speed Patroling", &m_speedPatroling, 0.01f, 20.0f, "%.2f", 0.01f);
        ImGui::SliderFloat("Speed Chasing", &m_speedChasing, 0.01f, 20.0f, "%.2f", 0.01f);
        ImGui::SliderFloat("Investigating Tolerance", &m_investigatingTolerance, 0.01f, 20.0f, "%.2f", 0.01f);
        //ImGui::SliderFloat2("", (float*)&m_ControlPointColors[0].m_Time, 0.25f, 10.0f);
    }
}


void CDron::patrol()
{
//    std::cout << " Dron " << GetName() << "patroling." << std::endl;
    if ((m_Destination == Vect3f{ 0.0f, -99.9f, 0.0f }) || (distanceBetweenTwoPoints(m_Position.x, m_Position.z, m_Destination.x,m_Destination.z)<=1.0f))
        GotoNextPoint();

    m_Movement.Lerp(m_Destination, m_speedPatroling);
    m_Position = m_Movement;
    this->SetForward(m_Destination - m_Position);
    m_PhysXManager.MoveCharacterController(GetName(), m_Position, PHYSX_UPDATE_STEP);
    this->SetPosition(m_Position);
}


void CDron::GotoNextPoint()
{
    if (m_pathChasingAux.size() > 0)  //Si se salió de la ruta para perseguir, se calcula de nuevo el camino de regreso
    {
        m_Destination = m_pathChasingAux[m_pathChasingAux.size()-1];
        m_Destination = Vect3f(m_Destination.x, m_Position.y, m_Destination.z);
        m_pathChasingAux.pop_back();
    }
    else
    {
        if (m_patrolPoints.size() == 0)
            return;

        m_Destination = GetPatrolPosition();
        m_DestPoint = (m_DestPoint + 1) % m_patrolPoints.size();
        m_Destination = Vect3f(m_Destination.x, m_Position.y, m_Destination.z);
    }
}

void CDron::chase()
{
    //  std::cout << " Dron " << GetName() << "chasing." << std::endl;

    if ((!m_Calculated) || (distanceBetweenTwoPoints(m_Position.x, m_Position.z, m_DestinationChase.x, m_DestinationChase.z) <= 0.2f))
        GotoNextPointChase();

    m_Movement.Lerp(m_DestinationChase, m_speedChasing);
    m_Position = m_Movement;
    this->SetForward(m_DestinationChase - m_Position);
    m_PhysXManager.MoveCharacterController(GetName(), m_Position, PHYSX_UPDATE_STEP);
    this->SetPosition(m_Position);
}

void CDron::GotoNextPointChase()
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
        m_DestinationChase = Vect3f(m_DestinationChase.x, m_Position.y, m_DestinationChase.z);
    }
    else
        ComputePath();
}


void CDron::ComputePath()
{
    //std::cout << " Dron " << GetName() << "chasing, calculó nuevo path" << std::endl;
    m_lastPositionView = m_PhysXManager.GetActorPosition("player");
    m_ppath = new CPathfinding(m_lastPositionView, m_Position, m_pnavMesh->GetName());
    m_DestPointChase = 0;
    m_Calculated = true;

    if (m_ppath->PathfindStep())
        m_pathChasing.push_back(m_lastPositionView);

}