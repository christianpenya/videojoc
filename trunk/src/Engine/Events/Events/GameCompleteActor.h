#pragma once

#ifndef _ENGINE_COMPLETEACTOR_20170831_H
#define _ENGINE_COMPLETEACTOR_20170831_H

#include "Events/Actor.h"

class CXMLElement;

class CGameCompleteActor : public CActor
{
private:

public:
    CGameCompleteActor();
    ~CGameCompleteActor();

    void Load(CXMLElement* aElement);
    void Act();
    void Update(float elapsedTime);
};

#endif //_ENGINE_DUMBACTOR_20170831_H
