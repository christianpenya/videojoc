#pragma once

#ifndef _ENGINE_AUDIOTRIGGERACTOR_20170910_H
#define _ENGINE_AUDIOTRIGGERACTOR_20170910_H

#include "Events/Actor.h"
#include "Sound/ISoundManager.h"

class CXMLElement;
class CSceneNode;

class CAudioTriggerActor : public CActor
{
public:
    CAudioTriggerActor();
    ~CAudioTriggerActor();

    void Load(CXMLElement* aElement);
    void Act();
    void Update(float elapsedTime);


    SoundEvent mSoundEvent;
};

#endif //_ENGINE_AUDIOTRIGGERACTOR_20170910_H
