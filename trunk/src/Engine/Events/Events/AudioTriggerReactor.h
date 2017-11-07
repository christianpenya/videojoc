#pragma once

#ifndef _ENGINE_CAudioTriggerReactor_20170831_H
#define _ENGINE_CAudioTriggerReactor_20170831_H

#include "Events/Reactor.h"
#include "Sound/ISoundManager.h"

class CXMLElement;

class CAudioTriggerReactor : public CReactor
{

private:

public:
    CAudioTriggerReactor();
    ~CAudioTriggerReactor();

    void Load(CXMLElement* aElement);
    void React();
    void Update(float elapsedTime);

    SoundEvent mSoundEvent;
};

#endif //_ENGINE_DUMBREACTOR_20170831_H
