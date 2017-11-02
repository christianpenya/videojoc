#include "EnemyAnimated.h"
#include "Graphics/Animation/AnimatedModelManager.h"
#include "cal3d.h"
#include "Graphics/Animation/SceneAnimatedModel.h"
#include "Graphics/Scenes/SceneManager.h"
#include "Engine/engine.h"
#include "Utils/Logger.h"
#include "Utils\EnumToString.h"
#include "ImGUI\imgui.h"
#include "XML\XML.h"
#include "Graphics/Mesh/MeshManager.h"

CEnemyAnimated::~CEnemyAnimated()
{
}

CEnemyAnimated::CEnemyAnimated(CXMLElement* aElement)
    : CSceneAnimatedModel(*aElement)
    , CEnemy(aElement)
    , state(CEnemyState::patroling)
    , m_SightDistance(aElement->GetAttribute<float>("sightDistance", 4.0f))
    , m_MaxDetectDistance(aElement->GetAttribute<float>("maxDetectDistance", 7.0f))
    , m_Destination(Vect3f{ 0.0f, -99.9f, 0.0f })
    , m_DestinationChase(Vect3f{ 0.0f, -99.9f, 0.0f })
    , m_Calculated(false)
    , m_Position(GetPosition())
    , m_Movement(GetPosition())
    , m_CalculateReturn(false)
    , m_State(Input::PATROL)
    , m_DetectAngle(aElement->GetAttribute<float>("detectAngle", 45.0f))
    , m_Corename(aElement->GetAttribute<std::string>("core", "dron"))
    , m_Group(aElement->GetAttribute<int>("group", 2))
{
    m_standby = aElement->GetAttribute<bool>("standby", false);

    m_Visible = aElement->GetAttribute<bool>("active", true);

    tinyxml2::XMLElement const* position = aElement->FirstChildElement();
    CTransform::SetForward(position->GetAttribute<Vect3f>("forward", Vect3f(0.0f, -1.0f, 0.0f)));

    CNavMeshManager& lNavMeshManager = CEngine::GetInstance().GetNavMeshManager();
    m_pnavMesh = lNavMeshManager(aElement->GetAttribute<std::string>("navmesh", "navMeshScene01"));
    m_ppath = new CPathfinding();

    //std::cout << " Dron " << GetName() << "inicializado." << std::endl;

    m_PhysXManager.AddCharacterController(GetName(), aElement->GetAttribute<float>("height", 1.1f), aElement->GetAttribute<float>("radiu", 0.17f), m_Position, Quatf(), "Default", 0.5f, m_Group);
    m_hear = false;
    m_enemydead = false;
}


Vect3f CEnemyAnimated::GetPatrolPosition()
{
    return (*this->GetParent())(m_patrolPoints[m_DestPoint].data())->GetPosition();
}

bool CEnemyAnimated::PlayerOnSight()
{
    float angulo = getAngle(m_Position.x, m_Height.y, m_Position.z, m_PhysXManager.GetActorPosition("player").x,m_Height.y, m_PhysXManager.GetActorPosition("player").z);
    return ((angulo <= m_DetectAngle) && (angulo >= -m_DetectAngle)) ? true : false;
}

void CEnemyAnimated::hearsPlayer()
{
    CPhysXManager::RaycastData* resultado = new CPhysXManager::RaycastData();
    bool hittedP = m_PhysXManager.Raycast(m_PhysXManager.GetActorPosition("player") + Vect3f(0.0f, 1.0f, 0.0f), m_Position + Vect3f(0.0f, 1.0f, 0.0f), m_Group, resultado);
    if (hittedP && (resultado->actor == "player"))
    {
        m_hear = true;
        std::cout << "Escucho prota" << std::endl;
    }
    else
        m_hear = false;
    delete resultado;

}

void CEnemyAnimated::dieEnemy()
{
    m_enemydead = true;
    ClearActiveAnimationCycle(0.5f);
    BlendCycle(2, 0.5f, 0.5f);

}

bool CEnemyAnimated::Update(float ElapsedTime)
{
    //if (!m_LevelController->GetTimePaused())
    {
        if (!m_enemydead)
        {

            //this->ClearCycle(1, 0.5);
            m_CalModel->update(ElapsedTime);
            m_ElapsedTime = ElapsedTime;
            Vect3f actorpos = m_PhysXManager.GetActorPosition("player");
            CPhysXManager::RaycastData* resultado = new CPhysXManager::RaycastData();
            bool hittedP = m_PhysXManager.Raycast(m_PhysXManager.GetActorPosition("player") + Vect3f(0.0f, 1.0f, 0.0f), m_Position + Vect3f(0.0f, 1.0f, 0.0f), m_Group, resultado);

            if (hittedP && (resultado->actor == "player"))
            {
                if (resultado->distance <= m_DeadDistance)
                {
                    if (PlayerOnSight())
                    {
                        m_State = Input::STOP;
                        m_LevelController.PlayerDetected(GetName());
                        std::cout << "Muere protagonista" << std::endl;
                    }
                }
                else if (resultado->distance <= m_SightDistance)
                {
                    if (PlayerOnSight() || m_hear)
                    {
                        m_State = Input::CHASE;
                        m_CalculateReturn = true;
                        //std::cout << "Chasing-" << std::endl;
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
                    m_hear = false;
                }
                //CEngine::GetInstance().GetEventManager()("Box hitted");
            }
            delete resultado;
            handleInput(m_State);
        }
    }
    return true;

}



std::shared_ptr<CPatrolingState> CEnemyState::patroling(new CPatrolingState);
std::shared_ptr<CChasingState> CEnemyState::chasing(new CChasingState);


void CPatrolingState::handleInput(CEnemyAnimated& enemy, Input input)
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

void CPatrolingState::update(CEnemyAnimated& enemy)
{
    LOG_INFO_APPLICATION("Patroling en enemy now");
}


void CChasingState::handleInput(CEnemyAnimated& enemy, Input input)
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

void CChasingState::update(CEnemyAnimated& enemy)
{
    LOG_INFO_APPLICATION("Chasing now");
}


void CEnemyAnimated::DrawImgui()
{
    if (ImGui::TreeNode(m_Name.c_str()))
    {
        ImGui::Checkbox("Visible", &m_Visible);
        ImGui::SliderFloat("Sight Distance", &m_SightDistance, 0.0f, 20.0f, "%.2f");
        ImGui::SliderFloat("Max Detect Distance", &m_MaxDetectDistance, 0.0f, 20.0f, "%.2f");
        ImGui::SliderFloat("Dead Distance", &m_DeadDistance, 0.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("Detect Angle", &m_DetectAngle, 0.0f, 360.0f, "%.2f");
        ImGui::Checkbox("Active", &m_Active);
        ImGui::SliderFloat3("Position", (float*)&m_Position, -500.0f, 500.0f, "%.2f");
        ImGui::SliderFloat3("Scale", (float*)&m_Scale, 0.0f, 100.0f, "%.2f");
        ImGui::TreePop();
    }
}