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
{
    m_Visible = aElement->GetAttribute<bool>("active", true);

    tinyxml2::XMLElement const* iTransformLight = aElement->FirstChildElement();
    CTransform::SetForward(iTransformLight->GetAttribute<Vect3f>("forward", Vect3f(0.0f, -1.0f, 0.0f)));
}


bool CEnemy::Update(float ElapsedTime)
{
    int choice = 1;
    const Input input1 = static_cast<Input>(choice - 1);
    handleInput(input1);
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

