#pragma once
#ifndef _ENGINE_CINEMATIC_OBJECT_KEY_20170213
#define _ENGINE_CINEMATIC_OBJECT_KEY_20170213

#include "CinematicKey.h"

class CCinematicObjectKey : public CCinematicKey
{
public:
    CCinematicObjectKey();
    virtual ~CCinematicObjectKey();
    virtual bool Load(const CXMLElement* aElement);
    GET_SET_REF(Vect3f, Rotation);
    GET_SET_REF(Vect3f, Scale);
private:
    Vect3f m_Rotation;
    Vect3f m_Scale;
private:
    DISALLOW_COPY_AND_ASSIGN(CCinematicObjectKey);
};

#endif