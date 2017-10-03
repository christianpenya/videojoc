#pragma once

#ifndef _ENGINE_AUDIOTRIGGERACTOR_20170910_H
#define _ENGINE_AUDIOTRIGGERACTOR_20170910_H

#include "Events/Actor.h"

class CXMLElement;
class CSceneNode;

class CAudioTriggerActor : public CActor
{
private:

public:
    CAudioTriggerActor();
    ~CAudioTriggerActor();

    void Load(CXMLElement* aElement);
    void Act();
    void Update(float elapsedTime);

    CSceneNode* mTrigger;
};

#endif //_ENGINE_AUDIOTRIGGERACTOR_20170910_H
