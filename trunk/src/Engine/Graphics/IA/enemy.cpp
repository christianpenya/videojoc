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
    : CSceneMesh(aElement)
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

bool CEnemy::Update(float ElapsedTime)
{
    bool hitted = m_PhysXManager.Raycast(m_Position, m_PhysXManager.GetActorPosition("player"), 0001, resultado);
    if (hitted && (resultado->actor == "player"))
    {
        /*        if (resultado->distance <= m_DeadDistance)
                {
                    choice = Input::STOP;
                    std::cout << "Muere protagonista" << std::endl;
                }*/
        if (resultado->distance <= m_SightDistance)
        {
            m_State = Input::CHASE;
            m_CalculateReturn = true;
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
}