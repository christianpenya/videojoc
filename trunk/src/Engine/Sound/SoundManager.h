#pragma once
#ifndef __H_SOUNDMANAGER__20170503
#define __H_SOUNDMANAGER__20170503

#include "AK\IBytes.h"
#include "AK\SoundEngine\Common\AkSoundEngine.h"
#include "AK\SoundEngine\Common\AkTypes.h"
#include "AK\SoundEngine\Common\AkMemoryMgr.h"
#include "AK\SoundEngine\Common\AkModule.h"
#include "AK\SoundEngine\Common\IAkStreamMgr.h"
#include "AK\Tools\Common\AkPlatformFuncs.h"
#include "AK\MusicEngine\Common\AkMusicEngine.h"

#include <string>
class CCamera;
class C3DElement;
class SoundEvent;
class SoundSwitchValue;
class SoundRTPC;
class SoundStateValue;

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

class ISoundManager
{
public:
    static ISoundManager* InstantiateSoundManager();
    virtual ~ISoundManager() {}

protected:
    std::string m_Path;

    virtual bool Init() = 0;
    virtual void Update(const CCamera *camera) = 0;
    virtual bool Load(const std::string &soundbanks_filename, const std::string &speakers_filename) = 0;
    virtual bool Reload() = 0;

    virtual bool LoadSoundBank(const std::string &bank) = 0;
    virtual bool UnloadSoundBank(const std::string &bank) = 0;

    virtual void RegisterSpeaker(const C3DElement* _speaker) = 0;
    virtual void UnregisterSpeaker(const C3DElement* _speaker) = 0;

    virtual void PlayEvent(const SoundEvent &_event) = 0;
    virtual void PlayEvent(const SoundEvent &_event, const std::string &_speaker) = 0;
    virtual void PlayEvent(const SoundEvent &_event, const C3DElement* _speaker) = 0;

    virtual void SetSwitch(const SoundSwitchValue &switchValue) = 0;
    virtual void SetSwitch(const SoundSwitchValue &switchValue, const std::string &_speaker) = 0;
    virtual void SetSwitch(const SoundSwitchValue &switchValue, const C3DElement* _speaker) = 0;

    virtual void BroadcastRTPCValue(const SoundRTPC &_rtpc, float value) = 0;
    virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value) = 0;
    virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value, const std::string &_speaker) = 0;
    virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value, const C3DElement* _speaker) = 0;

    virtual void BroadcastState(const SoundStateValue &_state) = 0;

    void SetPath(const std::string &path)
    {
        m_Path = path;
    }
};

#endif // __H_SOUNDMANAGER__20170503