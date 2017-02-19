#pragma once
#ifndef _ENGINE_CINEMATIC_CAMERA_KEY_20170213
#define _ENGINE_CINEMATIC_CAMERA_KEY_20170213

#include "CinematicKey.h"

class CCinematicCameraKey : public CCinematicKey
{
public:
    CCinematicCameraKey();
    virtual ~CCinematicCameraKey();
    virtual bool Load(const CXMLElement* aElement);
    GET_SET_REF(float, NearPlane);
    GET_SET_REF(float, FarPlane);
    GET_SET_REF(float, FOV);
    GET_SET_REF(Vect3f, Eye);
    GET_SET_REF(Vect3f, LookAt);
    GET_SET_REF(Vect3f, Up);
private:
    float m_NearPlane;
    float m_FarPlane;
    float m_FOV;
    Vect3f m_Eye;
    Vect3f m_LookAt;
    Vect3f m_Up;
private:
    DISALLOW_COPY_AND_ASSIGN(CCinematicCameraKey);
};

#endif