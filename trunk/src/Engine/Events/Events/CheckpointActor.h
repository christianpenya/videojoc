#pragma once

#ifndef _ENGINE_CHECKPOINTACTOR_20170831_H
#define _ENGINE_CHECKPOINTACTOR_20170831_H

#include "Events/Actor.h"
#include "Math/Quaternion.h"
class CXMLElement;



class CCheckpointActor : public CActor
{
private:
    Vect3f m_Position = Vect3f(0, 0, 0);
    Quatf m_Rotation =  Quatf(0,0,0,0);
public:
    CCheckpointActor();
    ~CCheckpointActor();

    void Load(CXMLElement* aElement);
    void Act();
    void Update(float elapsedTime);
};

#endif