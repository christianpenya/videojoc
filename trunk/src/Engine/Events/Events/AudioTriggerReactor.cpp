

#include <string>
#include "XML/XML.h"
#include "Utils/Logger.h"
#include "CheckpointReactor.h"
#include "AudioTriggerReactor.h"
#include "Engine/Engine.h"

CAudioTriggerReactor::CAudioTriggerReactor() {}
CAudioTriggerReactor::~CAudioTriggerReactor() {}

void CAudioTriggerReactor::Load(CXMLElement* aElement)
{
    //LOG_INFO_APPLICATION("Loading! I'm loading!");
    std::string lObjectName = aElement->GetAttribute<std::string>("obj_name", "");

    assert(lObjectName != "");

    mSoundEvent.eventName = lObjectName;
}

void CAudioTriggerReactor::React()
{
    //LOG_INFO_APPLICATION("Reacting! I'm reacting!");
    //LOG_INFO_APPLICATION("ACTING! I'M ACTING!");
    CEngine::GetInstance().m_SoundManager->PlayEvent(mSoundEvent, true);

}

void CAudioTriggerReactor::Update(float elapsedTime)
{
    //LOG_INFO_APPLICATION("React updating!");
    m_Finished = true;
}