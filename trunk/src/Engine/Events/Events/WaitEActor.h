#pragma once

#ifndef _ENGINE_WaitActor_20170831_H
#define _ENGINE_WaitActor_20170831_H

#include "Events/Actor.h"

#include "GUI/GUIPosition.h"
#include "Input/ActionManager.h"

class CXMLElement;
class CSceneNode;

class CWaitEActor : public CActor
{

public:
    CWaitEActor();
    ~CWaitEActor();

    void Load(CXMLElement* aElement);
    void Act();
    void Update(float elapsedTime);


    CActionManager *ActionMan;
    float mElapsed = 0;
    bool pressed;
};

#endif //_ENGINE_OPENDOORREACTOR_20170831_H
