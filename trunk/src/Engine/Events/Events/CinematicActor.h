#pragma once

#ifndef _ENGINE_CinematicActor_20170831_H
#define _ENGINE_CinematicActor_20170831_H

#include "Events/Actor.h"
#include "Graphics/Cinematics/CinematicsManager.h"

class CXMLElement;
class CSceneNode;

class CCinematicActor : public CActor
{

public:
    CCinematicActor();
    ~CCinematicActor();

    void Load(CXMLElement* aElement);
    void Act();
    void Update(float elapsedTime);


    CCinematicManager* cinematicMan = nullptr;
    std::string cinName;
};

#endif //_ENGINE_OPENDOORREACTOR_20170831_H
