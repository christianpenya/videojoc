#include "enemy.h"
#include "Engine/engine.h"
#include "Utils/Logger.h"
#include "Utils\EnumToString.h"
#include "ImGUI\imgui.h"
#include "XML\XML.h"
#include "Graphics/Mesh/MeshManager.h"

CEnemy::~CEnemy()
{
}

CEnemy::CEnemy(CXMLElement* aElement, EEnemyType aEnemyType)
    : //CSceneAnimatedModel(*aElement)
      CSceneMesh(aElement)
      //, mMesh(CEngine::GetInstance().GetMeshManager().GetMesh(aElement->GetAttribute<std::string>("mesh", "")))
    , state(CEnemyState::patroling)
    , m_PhysXManager(CEngine::GetInstance().GetPhysXManager())
    , resultado(new CPhysXManager::RaycastData())
    , m_SightDistance(aElement->GetAttribute<float>("sightDistance", 4.0f))
    , m_MaxDetectDistance(aElement->GetAttribute<float>("maxDetectDistance", 7.0f))
    , m_DeadDistance(aElement->GetAttribute<float>("deadDistance", 1.8f))
    , m_Destination(Vect3f{ 0.0f, -99.9f, 0.0f })
    , m_DestinationChase(Vect3f{ 0.0f, -99.9f, 0.0f })
    , m_Calculated(false)
    , m_Position(GetPosition())
    , m_Movement(GetPosition())
    , m_CalculateReturn(false)
    , m_State(Input::PATROL)
    , m_DetectAngle(aElement->GetAttribute<float>("detectAngle", 45.0f))

{
    m_Visible = aElement->GetAttribute<bool>("active", true);

    tinyxml2::XMLElement const* iTransformLight = aElement->FirstChildElement();
    CTransform::SetForward(iTransformLight->GetAttribute<Vect3f>("forward", Vect3f(0.0f, -1.0f, 0.0f)));

    CNavMeshManager& lNavMeshManager = CEngine::GetInstance().GetNavMeshManager();
    m_pnavMesh = lNavMeshManager(aElement->GetAttribute<std::string>("navmesh", "navMeshScene01"));
    m_ppath = new CPathfinding();

}

double CEnemy::distanceBetweenTwoPoints(double x, double y, double a, double b)
{
    return sqrt(pow(x - a, 2) + pow(y - b, 2));
}

Vect3f CEnemy::GetPatrolPosition()
{
    return (*this->GetParent())(m_patrolPoints[m_DestPoint].data())->GetPosition();
}

bool CEnemy::PlayerOnSight()
{
    float angulo = getAngle(m_Position.x, m_Position.y, m_Position.z, m_PhysXManager.GetActorPosition("player").x, m_PhysXManager.GetActorPosition("player").y, m_PhysXManager.GetActorPosition("player").z);
    return ((angulo <= m_DetectAngle) && (angulo >= -m_DetectAngle)) ? true : false;
}


bool CEnemy::Update(float ElapsedTime)
{
    bool hitted = m_PhysXManager.Raycast(m_Position, m_PhysXManager.GetActorPosition("player"), 0001, resultado);

    if (hitted && (resultado->actor == "player"))
    {
        if (resultado->distance <= m_DeadDistance)
        {
            if (PlayerOnSight())
            {
                m_State = Input::STOP;
                std::cout << "Muere protagonista" << std::endl;
            }
        }
        if (resultado->distance <= m_SightDistance)
        {
            if (PlayerOnSight())
            {
                m_State = Input::CHASE;
                m_CalculateReturn = true;
            }
        }
        else if (resultado->distance >= m_MaxDetectDistance)
        {
            if (m_CalculateReturn)
            {
                m_ppath = new CPathfinding(m_Position, GetPatrolPosition(), m_pnavMesh->GetName());
                bool encontro = m_ppath->PathfindStep();
                if (encontro)
                {
                    m_pathChasingAux = m_ppath->GetPath();
                    if (m_pathChasingAux.size() > 0)
                    {
                        m_Destination = m_pathChasingAux[m_pathChasingAux.size() - 1];
                        m_pathChasingAux.pop_back();
                    }
                    m_CalculateReturn = false;
                }
            }
            m_State = Input::PATROL;
            m_Calculated = false;
            m_ppath = new CPathfinding();
        }
        //CEngine::GetInstance().GetEventManager()("Box hitted");
    }
    handleInput(m_State);
    return true;
}



std::shared_ptr<CPatrolingState> CEnemyState::patroling(new CPatrolingState);
std::shared_ptr<CChasingState> CEnemyState::chasing(new CChasingState);


void CPatrolingState::handleInput(CEnemy& enemy, Input input)
{
    switch (input)
    {
    case CHASE:
        enemy.changeState(std::shared_ptr<CChasingState>(new CChasingState));
        return enemy.chase();
    case PATROL:
        enemy.changeState(std::shared_ptr<CPatrolingState>(new CPatrolingState));
        return enemy.patrol();
    default:
        update(enemy);
    }


}

void CPatrolingState::update(CEnemy& enemy)
{
    LOG_INFO_APPLICATION("Patroling en enemy now");
}


void CChasingState::handleInput(CEnemy& enemy, Input input)
{
    switch (input)
    {
    case CHASE:
        enemy.changeState(std::shared_ptr<CChasingState>(new CChasingState));
        return enemy.chase();
    case PATROL:
        enemy.changeState(std::shared_ptr<CPatrolingState>(new CPatrolingState));
        return enemy.patrol();
    default:
        update(enemy);
    }

}

void CChasingState::update(CEnemy& enemy)
{
    LOG_INFO_APPLICATION("Chasing now");
}


void CEnemy::DrawImgui()
{
    if (ImGui::TreeNode(m_Name.c_str()))
    {
        ImGui::Checkbox("Visible", &m_Visible);
        ImGui::SliderFloat("Sight Distance", &m_SightDistance, 0.0f, 20.0f,"%.2f");
        ImGui::SliderFloat("Max Detect Distance", &m_MaxDetectDistance, 0.0f, 20.0f, "%.2f");
        ImGui::SliderFloat("Dead Distance", &m_DeadDistance, 0.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("Detect Angle", &m_DetectAngle, 0.0f, 360.0f, "%.2f");
        ImGui::Checkbox("Active", &m_Active);
        ImGui::SliderFloat3("Position", (float*)&m_Position, -500.0f, 500.0f, "%.2f");
        ImGui::SliderFloat3("Scale", (float*)&m_Scale, 0.0f, 100.0f, "%.2f");
        ImGui::TreePop();
    }
}


float CEnemy::getAngle(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float theta = atan2(z1 - z2, x1 - x2);
    return -theta * 180 / 3.1415926;
}
float CEnemy::getAngle2(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
    float dist2 = sqrt(pow(x1 - x2, 2) + pow(z1 - z2, 2));
    return acos(dist2 / dist) * 180 / 3.1415926;
}