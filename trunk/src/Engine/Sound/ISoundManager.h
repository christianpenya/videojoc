#pragma once
#ifndef __H_ISOUNDMANAGER__20170503
#define __H_ISOUNDMANAGER__20170503

#define UNICODE // windows hack =D
// the "good" solution is to set "Properties -> General -> Character Set"
// to unicode, but that may break other parts of your project

#include <string>
#include <vector>
#include <unordered_map>

class CCamera;
class CSceneNode;
class SoundEvent;
class SoundSwitchValue;
class SoundRTPC;
class SoundStateValue;

class ISoundManager
{
public:
    virtual ~ISoundManager() {}

    static ISoundManager* InstantiateSoundManager();

    virtual void SetPath(const std::string &path) = 0;

    virtual bool Init() = 0;
    virtual void Update(const CCamera *camera) = 0;
    virtual bool Load(const std::string &soundbanks_filename, const std::string &speakers_filename) = 0;
    virtual bool Reload() = 0;

    /*
    virtual bool LoadSoundBank(const std::string &bank) = 0;
    virtual bool UnloadSoundBank(const std::string &bank) = 0;

    virtual void RegisterSpeaker(CSceneNode* _speaker) = 0;
    virtual void UnregisterSpeaker(const CSceneNode* _speaker) = 0;

    virtual void PlayEvent(const SoundEvent &_event) = 0;
    virtual void PlayEvent(const SoundEvent &_event, const std::string &_speaker) = 0;
    virtual void PlayEvent(const SoundEvent &_event, const CSceneNode* _speaker) = 0;

    virtual void SetSwitch(const SoundSwitchValue &switchValue) = 0;
    virtual void SetSwitch(const SoundSwitchValue &switchValue, const std::string &_speaker) = 0;
    virtual void SetSwitch(const SoundSwitchValue &switchValue, const CSceneNode* _speaker) = 0;

    virtual void BroadcastRTPCValue(const SoundRTPC &_rtpc, float value) = 0;
    virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value) = 0;
    virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value, const std::string &_speaker) = 0;
    virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value, const CSceneNode* _speaker) = 0;

    virtual void BroadcastState(const SoundStateValue &_state) = 0;

    virtual void Terminate() = 0;
    virtual void Clean() = 0;
    virtual bool LoadSoundBanksXML() = 0;
    virtual bool LoadSpeakersXML() = 0;
    */

protected:
    ISoundManager() {}
    std::string m_Path;

};

#endif // __H_ISOUNDMANAGER__20170503
