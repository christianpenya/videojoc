#include "Sound/SoundManager.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "XML/XML.h"
#include "Graphics/Camera/CameraController.h"

CSoundManager::CSoundManager() {}
CSoundManager::~CSoundManager() {}

ISoundManager* ISoundManager::InstantiateSoundManager()
{
    return new CSoundManager();
}

void CSoundManager::SetPath(const std::string &path)
{
    m_Path = path;
}

bool CSoundManager::Init()
{
    // Initialize audio engine
    // Memory.
    AkMemSettings memSettings;

    memSettings.uMaxNumPools = 20;

    // Streaming.
    AkStreamMgrSettings stmSettings;
    AK::StreamMgr::GetDefaultSettings( stmSettings );

    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

    AkInitSettings l_InitSettings;
    AkPlatformInitSettings l_platInitSetings;
    AK::SoundEngine::GetDefaultInitSettings(l_InitSettings);
    AK::SoundEngine::GetDefaultPlatformInitSettings(l_platInitSetings);

    // Setting pool sizes for this game. Here, allow for user content; every game should determine its own optimal values.
    l_InitSettings.uDefaultPoolSize				=  512*1024;
    l_InitSettings.uMaxNumPaths = 16;


    l_platInitSetings.uLEngineDefaultPoolSize	=  256*1024;

    AkMusicSettings musicInit;
    AK::MusicEngine::GetDefaultInitSettings(musicInit);

    AKRESULT eResult = AK::SOUNDENGINE_DLL::Init(&memSettings,
                       &stmSettings,
                       &deviceSettings,
                       &l_InitSettings,
                       &l_platInitSetings,
                       &musicInit );

    if( eResult != AK_Success )
    {
        // Then, we will run the game without sound
        AK::SOUNDENGINE_DLL::Term();
        return false;
    }

    // load initialization and main soundbanks

    //AK::SOUNDENGINE_DLL::SetBasePath(L"soundbanks\\Windows\\");
    //AK::StreamMgr::SetCurrentLanguage( L"English(US)" );

    return true;
}

void CSoundManager::Update(CCameraController *camera)
{
    // Update dynamic speaker position
    for (auto it : m_GameObjectSpeakers)
    {
        Vect3f l_SpeakerPosition = it.first->GetPosition();
        Vect3f l_SpeakerForward = it.first->GetForward();
        Vect3f l_SpeakerUp = it.first->GetUp();

        AkVector l_SoundPositionAkVector;
        l_SoundPositionAkVector.X = -l_SpeakerPosition.x;
        l_SoundPositionAkVector.Y = l_SpeakerPosition.y;
        l_SoundPositionAkVector.Z = l_SpeakerPosition.z;

        AkSoundPosition l_SoundPosition = {};
        l_SoundPosition.SetPosition(l_SoundPositionAkVector);

        AkVector l_SoundForward;
        l_SoundForward.X = -l_SpeakerForward.x;
        l_SoundForward.Y = l_SpeakerForward.y;
        l_SoundForward.Z = l_SpeakerForward.z;

        AkVector l_SoundUp;
        l_SoundUp.X = -l_SpeakerUp.x;
        l_SoundUp.Y = l_SpeakerUp.y;
        l_SoundUp.Z = l_SpeakerUp.z;

        l_SoundPosition.SetOrientation(l_SoundForward, l_SoundUp);

        AK::SoundEngine::SetPosition(it.second, l_SoundPosition);
    }

    //Update listener
    SetListenerPosition(camera);

    //Update wwise
    AK::SOUNDENGINE_DLL::Tick();
}

void CSoundManager::SetListenerPosition(CCameraController *camera)
{
    Vect3f l_Position = camera->GetPosition();
    Vect3f l_Forward = camera->GetFront();
    Vect3f l_Up = camera->GetUp();

    AkVector l_SoundPositionAkVector;
    l_SoundPositionAkVector.X = -l_Position.x;
    l_SoundPositionAkVector.Y = l_Position.y;
    l_SoundPositionAkVector.Z = l_Position.z;

    AkListenerPosition l_ListenerPosition = {};
    l_ListenerPosition.SetPosition(l_SoundPositionAkVector);

    AkVector l_SoundForward;
    l_SoundForward.X = -l_Forward.x;
    l_SoundForward.Y = l_Forward.y;
    l_SoundForward.Z = l_Forward.z;

    AkVector l_SoundUp;
    l_SoundUp.X = -l_Up.x;
    l_SoundUp.Y = l_Up.y;
    l_SoundUp.Z = l_Up.z;

    l_ListenerPosition.SetOrientation(l_SoundForward, l_SoundUp);
    AK::SoundEngine::SetListenerPosition(l_ListenerPosition);
}

bool CSoundManager::Load(const std::string &soundbanks_filename, const std::string &speakers_filename)
{
    m_SoundBanksFilename = soundbanks_filename;
    m_SpeakersFilename = speakers_filename;

    bool l_IsOk = true;
    l_IsOk = LoadSoundBanksXML();
    l_IsOk &= LoadSpeakersXML();

    return l_IsOk;
}

bool CSoundManager::Reload()
{
    return false;
}

bool CSoundManager::LoadSoundBank(const std::string &bank)
{
    AkBankID bankID;
    AKRESULT retValue;

    retValue = AK::SoundEngine::LoadBank(bank.c_str(), AK_DEFAULT_POOL_ID, bankID);

    if (retValue != AK_Success)
    {
        return false;
    }

    return true;
}

bool CSoundManager::UnloadSoundBank(const std::string &bank)
{
    AKRESULT retValue;

    retValue = AK::SoundEngine::UnloadBank(bank.c_str(), nullptr);
    if (retValue != AK_Success)
    {
        return false;
    }

    return true;
}

void CSoundManager::RegisterSpeaker(CSceneNode* _speaker)
{
    //assert(m_GameObjectSpeakers.find(_speaker) == m_GameObjectSpeakers.end());
    if (m_GameObjectSpeakers.find(_speaker) != m_GameObjectSpeakers.end())
    {
        UnregisterSpeaker(_speaker);
    }

    AkGameObjectID id = GenerateObjectID();
    m_GameObjectSpeakers[_speaker] = id;

    Vect3f l_SpeakerPosition = _speaker->GetPosition();
    Vect3f l_SpeakerForward = _speaker->GetForward();
    Vect3f l_SpeakerUp = _speaker->GetUp();

    AkVector l_SoundPositionAkVector;
    l_SoundPositionAkVector.X = -l_SpeakerPosition.x;
    l_SoundPositionAkVector.Y = l_SpeakerPosition.y;
    l_SoundPositionAkVector.Z = l_SpeakerPosition.z;

    AkSoundPosition l_SoundPosition = {};
    l_SoundPosition.SetPosition(l_SoundPositionAkVector);

    AkVector l_SoundForward;
    l_SoundForward.X = -l_SpeakerForward.x;
    l_SoundForward.Y = l_SpeakerForward.y;
    l_SoundForward.Z = l_SpeakerForward.z;

    AkVector l_SoundUp;
    l_SoundUp.X = -l_SpeakerUp.x;
    l_SoundUp.Y = l_SpeakerUp.y;
    l_SoundUp.Z = l_SpeakerUp.z;

    l_SoundPosition.SetOrientation(l_SoundForward, l_SoundUp);

    AK::SoundEngine::RegisterGameObj(id);
    AK::SoundEngine::SetPosition(id, l_SoundPosition);
}

void CSoundManager::UnregisterSpeaker(const CSceneNode* _speaker)
{
    auto it = m_GameObjectSpeakers.find(_speaker);
    if (it != m_GameObjectSpeakers.end())
    {
        AK::SoundEngine::UnregisterGameObj(it->second);
        m_FreeObjectsIDs.push_back(it->second);
        m_GameObjectSpeakers.erase(it);
    }
    else
    {
        assert(false);
    }
}

void CSoundManager::PlayEvent(SoundEvent &_event, const AkGameObjectID &id)
{
    AK::SoundEngine::PostEvent(_event.eventName.c_str(), id);
}

void CSoundManager::PlayEvent(SoundEvent &_event)
{
    PlayEvent(_event, m_DefaultSpeakerId);
}

void CSoundManager::PlayEvent(SoundEvent &_event, const std::string &_speaker)
{
    auto it = m_NamedSpeakers.find(_speaker);
    if (it != m_NamedSpeakers.end())
    {
        PlayEvent(_event, it->second);
    }
    else
    {
        assert(false);
    }
}

void CSoundManager::PlayEvent(SoundEvent &_event, const CSceneNode* _speaker)
{
    auto it = m_GameObjectSpeakers.find(_speaker);
    if (it != m_GameObjectSpeakers.end())
    {
        PlayEvent(_event, it->second);
    }
    else
    {
        assert(false);
    }
}
void CSoundManager::SetSwitch(SoundSwitchValue &switchValue, const AkGameObjectID &id)
{
    AKRESULT res = AK::SoundEngine::SetSwitch(switchValue.soundSwitch.switchName.c_str(), switchValue.valueName.c_str(), id);
    assert(res);
}

void CSoundManager::SetSwitch(SoundSwitchValue &switchValue)
{
    SetSwitch(switchValue, m_DefaultSpeakerId);
}

void CSoundManager::SetSwitch(SoundSwitchValue &switchValue, const std::string &_speaker)
{
    auto it = m_NamedSpeakers.find(_speaker);
    if (it != m_NamedSpeakers.end())
    {
        SetSwitch(switchValue, it->second);
    }
    else
    {
        assert(false);
    }
}

void CSoundManager::SetSwitch(SoundSwitchValue &switchValue, const CSceneNode* _speaker)
{
    auto it = m_GameObjectSpeakers.find(_speaker);
    if (it != m_GameObjectSpeakers.end())
    {
        SetSwitch(switchValue, it->second);
    }
    else
    {
        assert(false);
    }
}

void CSoundManager::SetRTPCValue(SoundRTPC &_rtpc, float value, const AkGameObjectID &id)
{
    AKRESULT res = AK::SoundEngine::SetRTPCValue(_rtpc.RTPCName.c_str(), (AkRtpcValue)value, id);
    assert(res);
}

void CSoundManager::SetRTPCValue(SoundRTPC &_rtpc, float value)
{
    SetRTPCValue(_rtpc, value, m_DefaultSpeakerId);
}

void CSoundManager::SetRTPCValue(SoundRTPC &_rtpc, float value, const std::string &_speaker)
{
    auto it = m_NamedSpeakers.find(_speaker);
    if (it != m_NamedSpeakers.end())
    {
        SetRTPCValue(_rtpc, value, it->second);
    }
    else
    {
        assert(false);
    }
}
void CSoundManager::SetRTPCValue(SoundRTPC &_rtpc, float value, const CSceneNode* _speaker)
{
    auto it = m_GameObjectSpeakers.find(_speaker);
    if (it != m_GameObjectSpeakers.end())
    {
        SetRTPCValue(_rtpc, value, it->second);
    }
    else
    {
        assert(false);
    }
}


void CSoundManager::BroadcastRTPCValue(const SoundRTPC &_rtpc, float value)
{
    AKRESULT res = AK::SoundEngine::SetRTPCValue(_rtpc.RTPCName.c_str(), (AkRtpcValue)value);
    assert(res);
}

void CSoundManager::BroadcastState(const SoundStateValue &_state)
{
    AKRESULT res = AK::SoundEngine::SetState(_state.soundState.stateName.c_str(), _state.valueName.c_str());
    assert(res);
}

AkGameObjectID CSoundManager::GenerateObjectID()
{
    AkGameObjectID result;
    if (m_FreeObjectsIDs.size() > 0)
    {
        result = m_FreeObjectsIDs.back();
        m_FreeObjectsIDs.pop_back();
    }
    else
    {
        result = ++m_LastGameObjectID;
    }
    return result;
}

void CSoundManager::Terminate()
{
    AK::SoundEngine::ClearBanks();
    AK::SoundEngine::UnregisterAllGameObj();

    AK::SOUNDENGINE_DLL::Term();
}

void CSoundManager::Clean()
{
    AK::SoundEngine::ClearBanks();

    for (auto it : m_NamedSpeakers)
    {
        AK::SoundEngine::UnregisterGameObj(it.second);
        m_FreeObjectsIDs.push_back(it.second);
    }

    m_NamedSpeakers.clear();
}

bool CSoundManager::LoadSoundBanksXML()
{
    InitBanks();

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError l_Error = doc.LoadFile((m_Path + m_SoundBanksFilename).c_str());

    tinyxml2::XMLElement* l_Element;

    if (l_Error == tinyxml2::XML_SUCCESS)
    {
        l_Element = doc.FirstChildElement("SoundBanks")->FirstChildElement();

        while (l_Element != NULL)
        {
            std::string l_Name = l_Element->GetAttribute<std::string>("name", "");
            LoadSoundBank(l_Name);
            l_Element = l_Element->NextSiblingElement();
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool CSoundManager::LoadSpeakersXML()
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError l_Error = doc.LoadFile((m_Path + m_SpeakersFilename).c_str());

    tinyxml2::XMLElement* l_Element;

    if (l_Error == tinyxml2::XML_SUCCESS)
    {
        l_Element = doc.FirstChildElement("Speakers")->FirstChildElement();

        while (l_Element != NULL)
        {
            std::string l_Name = l_Element->GetAttribute<std::string>("name", "");

            CTransform lTransform;
            lTransform.SetForward(l_Element->GetAttribute<Vect3f>("forward", Vect3f(1.0f, 1.0f, 1.0f)));
            lTransform.SetPosition(l_Element->GetAttribute<Vect3f>("position", Vect3f(0.0f, 0.0f, 0.0f)));

            AkVector l_SoundPositionAkVector;
            l_SoundPositionAkVector.X = -lTransform.GetPosition().x;
            l_SoundPositionAkVector.Y = lTransform.GetPosition().y;
            l_SoundPositionAkVector.Z = lTransform.GetPosition().z;

            AkSoundPosition l_SoundPosition = {};
            l_SoundPosition.SetPosition(l_SoundPositionAkVector);

            AkVector l_SoundForward;
            l_SoundForward.X = -lTransform.GetForward().x;
            l_SoundForward.Y = lTransform.GetForward().y;
            l_SoundForward.Z = lTransform.GetForward().z;

            AkVector l_SoundUp;
            l_SoundUp.X = -lTransform.GetUp().x;
            l_SoundUp.Y = lTransform.GetUp().y;
            l_SoundUp.Z = lTransform.GetUp().z;

            l_SoundPosition.SetOrientation(l_SoundForward, l_SoundUp);

            AkGameObjectID id = GenerateObjectID();
            m_NamedSpeakers[l_Name] = id;
            AK::SoundEngine::RegisterGameObj(id);
            AK::SoundEngine::SetPosition(id, l_SoundPosition);
            l_Element = l_Element->NextSiblingElement();
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool CSoundManager::InitBanks()
{
    // load initialization and main soundbanks
    AkOSChar *path;
    AKRESULT retValue;
    CONVERT_CHAR_TO_OSCHAR(m_Path.c_str(), path);
    retValue = AK::SOUNDENGINE_DLL::SetBasePath(path);
    retValue = AK::StreamMgr::SetCurrentLanguage(L"English(US)");

    AkBankID bankID;
    retValue = AK::SoundEngine::LoadBank("Init.bnk", AK_DEFAULT_POOL_ID, bankID);
    if (retValue != AK_Success)
    {
        return false;
    }

    return true;
}
