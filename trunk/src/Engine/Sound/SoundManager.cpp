#include "Sound/SoundManager.h"

#include <cassert>

#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
#include <AkFilePackageLowLevelIOBlocking.h>                    // Sample low-level I/O implementation
#include <AK/SoundEngine/Common/AkSoundEngine.h>                // Sound engine
#include <AK/MusicEngine/Common/AkMusicEngine.h>                // Music Engine

CSoundManager::CSoundManager() {}
CSoundManager::~CSoundManager() {}

ISoundManager* CSoundManager::InstantiateSoundManager()
{
    return new CSoundManager();
}

void CSoundManager::SetPath(const std::string &path)
{
    m_Path = path;
}

bool CSoundManager::Init()
{
    //
    // Create and initialize an instance of the default memory manager. Note
    // that you can override the default memory manager with your own. Refer
    // to the SDK documentation for more information.
    //

    AkMemSettings memSettings;
    memSettings.uMaxNumPools = 20;

    if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
    {
        assert(!"Could not create the memory manager.");
        return false;
    }

    //
    // Create and initialize an instance of the default streaming manager. Note
    // that you can override the default streaming manager with your own. Refer
    // to the SDK documentation for more information.
    //

    AkStreamMgrSettings stmSettings;
    AK::StreamMgr::GetDefaultSettings(stmSettings);

    // Customize the Stream Manager settings here.

    if (!AK::StreamMgr::Create(stmSettings))
    {
        assert(!"Could not create the Streaming Manager");
        return false;
    }

    //
    // Create a streaming device with blocking low-level I/O handshaking.
    // Note that you can override the default low-level I/O module with your own. Refer
    // to the SDK documentation for more information.
    //
    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

    // Customize the streaming device settings here.

    // CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
    // in the Stream Manager, and registers itself as the File Location Resolver.
    if (g_lowLevelIO->Init(deviceSettings) != AK_Success)
    {
        assert(!"Could not create the streaming device and Low-Level I/O system");
        return false;
    }

    //
    // Create the Sound Engine
    // Using default initialization parameters
    //

    AkInitSettings initSettings;
    AkPlatformInitSettings platformInitSettings;
    AK::SoundEngine::GetDefaultInitSettings(initSettings);
    AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

    if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
    {
        assert(!"Could not initialize the Sound Engine.");
        return false;
    }

    //
    // Initialize the music engine
    // Using default initialization parameters
    //

    AkMusicSettings musicInit;
    AK::MusicEngine::GetDefaultInitSettings(musicInit);

    if (AK::MusicEngine::Init(&musicInit) != AK_Success)
    {
        assert(!"Could not initialize the Music Engine.");
        return false;
    }

    return true;
}

void CSoundManager::Update(const CCamera *camera)
{

}

bool CSoundManager::Load(const std::string &soundbanks_filename, const std::string &speakers_filename)
{
    return false;
}

bool CSoundManager::Reload()
{
    return false;
}

/*
#include "SoundManager.h"

#include <cassert>
#include "Math/Vector3.h"
#include "Graphics/Scenes/SceneNode.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
#include <AkFilePackageLowLevelIOBlocking.h>                    // Sample low-level I/O implementation
#include <AK/SoundEngine/Common/AkSoundEngine.h>                // Sound engine
#include <AK/MusicEngine/Common/AkMusicEngine.h>                // Music Engine

class CSoundManager : public ISoundManager
{
public:
    //static ISoundManager* InstantiateSoundManager();
    virtual ~CSoundManager() {}

    AkGameObjectID m_LastGameObjectId;
    std::vector<AkGameObjectID> m_FreeObjectIDs;

    AkGameObjectID m_DefaultSpeakerId;
    std::unordered_map<std::string, AkGameObjectID> m_NamedSpeakers;
    std::unordered_map<const CSceneNode*, AkGameObjectID> m_GameObjectSpeakers;

    std::string m_SoundBanksFilename;
    std::string m_SpeakersFilename;

    virtual bool Init();
    virtual bool InitBanks();
    virtual void Update(const CCamera *camera);
    virtual bool Load(const std::string &soundbanks_filename, const std::string &speakers_filename);
    virtual bool Reload();

    virtual bool LoadSoundBank(const std::string &bank);
    virtual bool UnloadSoundBank(const std::string &bank);

    virtual void RegisterSpeaker(CSceneNode* _speaker);
    virtual void UnregisterSpeaker(const CSceneNode* _speaker);

    virtual void PlayEvent(const SoundEvent &_event);
    virtual void PlayEvent(const SoundEvent &_event, const std::string &_speaker);
    virtual void PlayEvent(const SoundEvent &_event, const CSceneNode* _speaker);

    virtual void SetSwitch(const SoundSwitchValue &switchValue);
    virtual void SetSwitch(const SoundSwitchValue &switchValue, const std::string &_speaker);
    virtual void SetSwitch(const SoundSwitchValue &switchValue, const CSceneNode* _speaker);

    virtual void BroadcastRTPCValue(const SoundRTPC &_rtpc, float value);
    virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value);
    virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value, const std::string &_speaker);
    virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value, const CSceneNode* _speaker);

    virtual void BroadcastState(const SoundStateValue &_state);
    virtual void SetPath(const std::string &path);
    virtual AkGameObjectID GenerateObjectId();

    virtual void Terminate();
    virtual void Clean();
    virtual bool LoadSoundBanksXML();
    virtual bool LoadSpeakersXML();
};


/*
void ProcessAudio()
{
    // Process bank requests, events, positions, RTPC, etc.
    AK::SoundEngine::RenderAudio();
}

void TermSoundEngine()
{
    //
    // Terminate the music engine
    //

    AK::MusicEngine::Term();

    //
    // Terminate the sound engine
    //

    AK::SoundEngine::Term();

    // Terminate the streaming device and streaming manager

    // CAkFilePackageLowLevelIOBlocking::Term() destroys its associated streaming device
    // that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
    g_lowLevelIO.Term();

    if (AK::IAkStreamMgr::Get())
        AK::IAkStreamMgr::Get()->Destroy();

    // Terminate the Memory Manager
    AK::MemoryMgr::Term();
}

AkGameObjectID CSoundManager::GenerateObjectId()
{
    AkGameObjectID result;

    if (m_FreeObjectIDs.size() > 0)
    {
        result = m_FreeObjectIDs.back();
        m_FreeObjectIDs.pop_back();
    }
    else
    {
        result = ++m_LastGameObjectId;
    }

    return result;
}

void CSoundManager::RegisterSpeaker(CSceneNode* _speaker)
{
    assert(m_GameObjectSpeakers.find(_speaker) == m_GameObjectSpeakers.end());

    AkGameObjectID id = GenerateObjectId();
    m_GameObjectSpeakers[_speaker] = id;

    Vect3f l_Position = _speaker->GetPosition();
    Vect3f l_Orientation = _speaker->GetForward();
    Vect3f l_Up = _speaker->GetUp();

    AkSoundPosition l_SoundPosition = {};
    l_SoundPosition.SetPosition(l_Position.x, l_Position.y, l_Position.z);
    l_SoundPosition.SetOrientation(l_Orientation.x, l_Orientation.y, l_Orientation.z, l_Up.x, l_Up.y, l_Up.z);

    AK::SoundEngine::RegisterGameObj(id);
    AK::SoundEngine::SetPosition(id, l_SoundPosition);
}

void CSoundManager::UnregisterSpeaker(const CSceneNode* _speaker)
{
    auto it = m_GameObjectSpeakers.find(_speaker);
    if (it != m_GameObjectSpeakers.end())
    {
        AK::SoundEngine::UnregisterGameObj(it->second);
        m_FreeObjectIDs.push_back(it->second);
        m_GameObjectSpeakers.erase(it);
    }
    else
    {
        assert(false);
    }
}

bool CSoundManager::Load(const std::string &soundbanks_filename, const std::string &speakers_filename)
{
    m_SoundBanksFilename = soundbanks_filename;
    m_SpeakersFilename = speakers_filename;

    bool l_Ok = true;
    l_Ok = LoadSoundBanksXML();
    l_Ok &= LoadSpeakersXML();

    return l_Ok;
}

bool CSoundManager::LoadSoundBanksXML()
{
    return false;
}

bool CSoundManager::LoadSpeakersXML()
{
    return false;
}

bool CSoundManager::InitBanks()
{
    AkOSChar *path;
    AKRESULT retValue;
    CONVERT_CHAR_TO_OSCHAR(m_Path.c_str(), path);
    AkBankID bankId;
    retValue = AK::StreamMgr::SetCurrentLanguage(L"English(US)");

    //TODO repassar, no acaba de ser igual que al pdf
    retValue = AK::SoundEngine::LoadBank(path, AK_DEFAULT_POOL_ID, bankId);

    if (retValue != AK_Success)
    {
        return false;
    }

    return true;
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
    AkBankID bankID;
    AKRESULT retValue;

    retValue = AK::SoundEngine::UnloadBank(bank.c_str(), nullptr);
    if (retValue != AK_Success)
    {
        return false;
    }

    return true;
}

void CSoundManager::Terminate()
{
    AK::SoundEngine::ClearBanks();
    AK::SoundEngine::UnregisterAllGameObj();
    AK::SoundEngine::Term();
}

void CSoundManager::Clean()
{
    AK::SoundEngine::ClearBanks();

    for (auto it : m_NamedSpeakers)
    {
        AK::SoundEngine::UnregisterGameObj(it.second);
        m_FreeObjectIDs.push_back(it.second);
    }

    m_NamedSpeakers.clear();
}
*/