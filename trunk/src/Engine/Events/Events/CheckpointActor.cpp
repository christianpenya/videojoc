#include "XML/XML.h"

#include "Engine/Engine.h"
#include "CheckpointActor.h"
#include"Events/LevelController.h"

CCheckpointActor::CCheckpointActor() {}
CCheckpointActor::~CCheckpointActor() {}
void CCheckpointActor::Load(CXMLElement* aElement)
{
    mObjectName = aElement->GetAttribute<std::string>("triggerName", "");
    CSceneNode* mTrigger = CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetLayer("triggers")->GetSceneNode(mObjectName);
    if (mTrigger)
    {
        m_Position = mTrigger->GetPosition();
        m_Rotation.QuatFromYawPitchRoll(mTrigger->GetYaw(),
                                        mTrigger->GetPitch(),
                                        mTrigger->GetRoll());

    }

}

void CCheckpointActor::Act()
{
    CSceneNode* mTrigger = CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetLayer("triggers")->GetSceneNode(mObjectName);
    if (mTrigger)
    {
        m_Position = mTrigger->GetPosition();
        m_Rotation.QuatFromYawPitchRoll(mTrigger->GetYaw(),
                                        mTrigger->GetPitch(),
                                        mTrigger->GetRoll());

    }
    CLevelController* m_Level = CEngine::GetInstance().m_LevelController;
    if (m_Level != nullptr)
    {
        m_Level->SetLastCheckpointP(m_Position);
        m_Level->SetLastCheckpointR(m_Rotation);
    }
}

void CCheckpointActor::Update(float elapsedTime)
{

    m_Finished = true;

}