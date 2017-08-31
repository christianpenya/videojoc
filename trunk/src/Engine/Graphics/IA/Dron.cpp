#include "Graphics\IA\Dron.h"
#include "Graphics\Mesh\MeshManager.h"
#include "Physx\PhysxManager.h"
#include "Engine\engine.h"
#include "Utils\Defines.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "Utils\EnumToString.h"
#include "Utils\Logger.h"
#include "XML\XML.h"


CDron::~CDron()
{

}

CDron::CDron(CXMLElement* aTreeNode)
    : CEnemy(aTreeNode, CEnemy::eDron)
    , m_PhysXManager(CEngine::GetInstance().GetPhysXManager())
    , m_SightDistance(aTreeNode->GetAttribute<float>("sightDistance", 4.0f))
    , m_MaxDetectDistance(aTreeNode->GetAttribute<float>("maxDetectDistance", 7.0f))
    , m_DeadDistance(aTreeNode->GetAttribute<float>("deadDistance", 1.8f))
    , m_DetectAngle(aTreeNode->GetAttribute<float>("detectAngle", 45.0f))
    , m_soundDetected(aTreeNode->GetAttribute<std::string>("soundDetected", ""))
    , m_speedPatroling(aTreeNode->GetAttribute<float>("speedPatroling", 0.8f))
    , m_speedChasing(aTreeNode->GetAttribute<float>("speedChasing", 1.0f))
    , m_investigatingTolerance(aTreeNode->GetAttribute<float>("investigatingTolerance", 1.0f))
    , m_Position(GetPosition())
    , m_Movement(GetPosition())
    , m_Destination(Vect3f{ 0.0f, -99.9f, 0.0f })
{

    CNavMeshManager& lNavMeshManager = CEngine::GetInstance().GetNavMeshManager();
    m_pnavMesh = lNavMeshManager(aTreeNode->GetAttribute<std::string>("navmesh", "navMeshScene01"));

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
    std::cout << " Dron " << GetName() << "patroling." << std::endl;

    if ((m_Destination == Vect3f{ 0.0f, -99.9f, 0.0f }) || (distanceBetweenTwoPoints(m_Position.x, m_Position.z, m_Destination.x,m_Destination.z)<=1.5f))
        GotoNextPoint();

    m_Movement.Lerp(m_Destination, 0.5f);
    m_Movement = m_Movement.GetNormalized();
    this->SetForward(m_Movement);

    m_Movement *= m_speedPatroling;
    m_Position = m_Position + m_Movement;
    m_PhysXManager.MoveCharacterController(GetName(), m_Movement, PHYSX_UPDATE_STEP);
    this->SetPosition(m_Position);
}

double CDron::distanceBetweenTwoPoints(double x, double y, double a, double b)
{
    return sqrt(pow(x - a, 2) + pow(y - b, 2));
}

void CDron::GotoNextPoint()
{
    if (m_patrolPoints.size() == 0)
        return;

    m_Destination = GetPatrolPosition();
    m_DestPoint = (m_DestPoint + 1) % m_patrolPoints.size();

}

Vect3f CDron::GetPatrolPosition()
{
    return (*this->GetParent())(m_patrolPoints[m_DestPoint].data())->GetPosition();
}
