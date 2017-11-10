#pragma once

#ifndef _ENGINE_AUDIOSWITCHACTOR_20171109_H
#define _ENGINE_AUDIOSWITCHACTOR_20171109_H

#include "Events/Actor.h"
#include "Sound/ISoundManager.h"

class CXMLElement;
class CSceneNode;

class CAudioSwitchActor : public CActor
{
public:
    CAudioSwitchActor();
    ~CAudioSwitchActor();

    void Load(CXMLElement* aElement);
    void Act();
    void Update(float elapsedTime);


    SoundEvent mSoundEvent;
    SoundSwitchValue mSoundSwitch;
};

#endif //_ENGINE_AUDIOSWITCHACTOR_20171109_H
