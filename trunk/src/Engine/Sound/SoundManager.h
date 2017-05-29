#pragma once
#ifndef __H_SOUNDMANAGER__20170516
#define __H_SOUNDMANAGER__20170516

#include "ISoundManager.h"

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

#include <..\samples\DynamicLibraries\AkSoundEngineDLL\AkSoundEngineDLL.h>

// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
namespace AK
{
#ifdef WIN32
void * AllocHook(size_t in_size)
{
    return malloc(in_size);
}
void FreeHook(void * in_ptr)
{
    free(in_ptr);
}
// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
// you may implement it with a simple malloc().
void * VirtualAllocHook(
    void * in_pMemAddress,
    size_t in_size,
    DWORD in_dwAllocationType,
    DWORD in_dwProtect
)
{
    return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
}
void VirtualFreeHook(
    void * in_pMemAddress,
    size_t in_size,
    DWORD in_dwFreeType
)
{
    VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
}
#endif
}

class CAkFilePackageLowLevelIOBlocking;

class CSoundManager : public ISoundManager
{
public:

    CSoundManager();
    virtual ~CSoundManager();

    virtual void SetPath(const std::string &path);

    virtual bool Init() override;
    virtual void Update(const CCamera *camera) override;
    virtual bool Load(const std::string &soundbanks_filename, const std::string &speakers_filename) override;
    virtual bool Reload() override;

    CAkFilePackageLowLevelIOBlocking* g_lowLevelIO;

    /*
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
    */

protected:

    //CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
};

#endif // __H_SOUNDMANAGER__20170516
