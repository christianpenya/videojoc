#include "AudioTriggerActor.h"

#include <string>
#include "XML/XML.h"
#include "Utils/Logger.h"

#include "Engine/Engine.h"
#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Scenes/Scene.h"
#include "Graphics/Scenes/Layer.h"
#include "Graphics/Scenes/SceneNode.h"
#include "Sound/ISoundManager.h"

#include "Utils/CheckedRelease.h"

CAudioTriggerActor::CAudioTriggerActor() {}
CAudioTriggerActor::~CAudioTriggerActor()
{
}

void CAudioTriggerActor::Load(CXMLElement* aElement)
{
    std::string lObjectName = aElement->GetAttribute<std::string>("obj_name", "");

    assert(lObjectName != "");

    mSoundEvent.eventName = lObjectName;

}

void CAudioTriggerActor::Act()
{
    //LOG_INFO_APPLICATION("ACTING! I'M ACTING!");
    CEngine::GetInstance().m_SoundManager->PlayEvent(mSoundEvent, true);
}

void CAudioTriggerActor::Update(float elapsedTime)
{
    //LOG_INFO_APPLICATION("ACTOR UPDATING");

    if (CEngine::GetInstance().m_SoundManager->NotifyEndOfEvent())
    {
        m_Finished = true;
    }
}