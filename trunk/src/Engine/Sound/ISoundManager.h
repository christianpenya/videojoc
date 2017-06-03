#pragma once
#ifndef __H_ISOUNDMANAGER__20170503
#define __H_ISOUNDMANAGER__20170503

#define UNICODE // windows hack =D

#include <string>
#include <vector>
#include <unordered_map>

class CCameraController;
class CSceneNode;
//class AkGameObjectID;

struct SoundState
{
    std::string stateName;
};

struct SoundStateValue
{
    SoundState soundState;
    std::string valueName;
};


struct SoundRTPC
{
    std::string RTPCName;
};

struct SoundSwitch
{
    std::string switchName;
};

struct SoundSwitchValue
{
    SoundSwitch soundSwitch;
    std::string valueName;
};

struct SoundEvent
{
    std::string eventName;
};

class ISoundManager
{
public:
    virtual ~ISoundManager() {}

    static ISoundManager* InstantiateSoundManager();

    virtual void SetPath(const std::string &path) = 0;

    virtual bool Init() = 0;
    virtual void Update(CCameraController *camera) = 0;
    virtual bool Load(const std::string &soundbanks_filename, const std::string &speakers_filename) = 0;
    virtual bool Reload() = 0;

    virtual bool LoadSoundBank(const std::string &bank) = 0;
    virtual bool UnloadSoundBank(const std::string &bank) = 0;

    virtual void RegisterSpeaker(CSceneNode* _speaker) = 0;
    virtual void UnregisterSpeaker(const CSceneNode* _speaker) = 0;

    virtual void PlayEvent(SoundEvent &_event) = 0;
    virtual void PlayEvent(SoundEvent &_event, const std::string &_speaker) = 0;
    virtual void PlayEvent(SoundEvent &_event, const CSceneNode* _speaker) = 0;

    virtual void SetSwitch(SoundSwitchValue &switchValue) = 0;
    virtual void SetSwitch(SoundSwitchValue &switchValue, const std::string &_speaker) = 0;
    virtual void SetSwitch(SoundSwitchValue &switchValue, const CSceneNode* _speaker) = 0;

    virtual void SetRTPCValue(SoundRTPC &_rtpc, float value) = 0;
    virtual void SetRTPCValue(SoundRTPC &_rtpc, float value, const std::string &_speaker) = 0;
    virtual void SetRTPCValue(SoundRTPC &_rtpc, float value, const CSceneNode* _speaker) = 0;

    virtual void BroadcastState(const SoundStateValue &_state) = 0;
    virtual void BroadcastRTPCValue(const SoundRTPC &_rtpc, float value) = 0;

    virtual void Terminate() = 0;
    virtual void Clean() = 0;
    virtual bool LoadSoundBanksXML() = 0;
    virtual bool LoadSpeakersXML() = 0;

protected:
    ISoundManager() {}
    std::string m_Path;

};

#endif // __H_ISOUNDMANAGER__20170503
