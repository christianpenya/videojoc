#pragma once

#ifndef _ENGINE_OPENDOORREACTOR_20170831_H
#define _ENGINE_OPENDOORREACTOR_20170831_H

#include "Events/Reactor.h"

class CXMLElement;
class CSceneNode;

class COpenDoorReactor : public CReactor
{

public:
    COpenDoorReactor();
    ~COpenDoorReactor();

    void Load(CXMLElement* aElement);
    void React();
    void Update(float elapsedTime);

    CSceneNode* mDoor;

};

#endif //_ENGINE_OPENDOORREACTOR_20170831_H
