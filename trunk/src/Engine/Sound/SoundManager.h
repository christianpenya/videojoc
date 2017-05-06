#pragma once
#ifndef __H_SOUNDMANAGER__20170503
#define __H_SOUNDMANAGER__20170503

#include <string>
class CCamera;
class C3DElement;
class SoundEvent;
class SoundSwitchValue;
class SoundRTPC;
class SoundStateValue;

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