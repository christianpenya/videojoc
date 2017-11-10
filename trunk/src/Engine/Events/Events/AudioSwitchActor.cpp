#include "AudioSwitchActor.h"

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

CAudioSwitchActor::CAudioSwitchActor() {}
CAudioSwitchActor::~CAudioSwitchActor() {}

void CAudioSwitchActor::Load(CXMLElement* aElement)
{
    std::string lObjectName = aElement->GetAttribute<std::string>("obj_name", "");
    std::string lSwitchTo = aElement->GetAttribute<std::string>("switch_to", "");

    assert(lObjectName != "");
    assert(lSwitchTo != "");

    mSoundEvent.eventName = lObjectName;

    SoundSwitch lSwitch;
    lSwitch.switchName = lObjectName;
    mSoundSwitch.soundSwitch = lSwitch;
    mSoundSwitch.valueName = lSwitchTo;
}

void CAudioSwitchActor::Act()
{
    CEngine::GetInstance().m_SoundManager->SetSwitch(mSoundSwitch);
    //CEngine::GetInstance().m_SoundManager->Terminate(mSoundEvent);
    CEngine::GetInstance().m_SoundManager->PlayEvent(mSoundEvent);
}

void CAudioSwitchActor::Update(float elapsedTime)
{
    m_Finished = true;
}