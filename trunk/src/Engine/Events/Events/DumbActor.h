#pragma once

#ifndef _ENGINE_DUMBACTOR_20170831_H
#define _ENGINE_DUMBACTOR_20170831_H

#include "Events/Actor.h"

class CXMLElement;

class CDumbActor : public CActor
{
private:

public:
    CDumbActor();
    ~CDumbActor();

    void Load(CXMLElement* aElement);
    void Act();
    void Update(float elapsedTime);
};

#endif //_ENGINE_DUMBACTOR_20170831_H
