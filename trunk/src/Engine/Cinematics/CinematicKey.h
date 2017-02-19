#pragma once
#ifndef _ENGINE_CINEMATIC_KEY_20170213
#define _ENGINE_CINEMATIC_KEY_20170213

#include "XML\tinyxml2\tinyxml2.h"
#include "Utils\TemplatedMapVector.h"
#include "Utils\Name.h"
#include <vector>
#include "Math\MathTypes.h"
#include "Math\Vector3.h"


class CCinematicKey
{
public:
    CCinematicKey();
    virtual ~CCinematicKey() = 0;
    virtual bool Load(const CXMLElement* aElement) = 0;
    GET_SET_REF(float, Time);
    GET_SET_REF(Vect3f, Position);
private:
    DISALLOW_COPY_AND_ASSIGN(CCinematicKey);
protected:
    float m_Time;
    Vect3f m_Position;
};


#endif