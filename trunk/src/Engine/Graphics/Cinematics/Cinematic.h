#pragma once
#ifndef _ENGINE_CINEMATIC_20170213
#define _ENGINE_CINEMATIC_20170213

#include "Utils/TemplatedMapVector.h"
#include "Utils/Name.h"

class CCinematicPlayer;
class CCinematicCameraPlayer;
class CCameraController;

XML_FORWARD_DECLARATION

class CCinematic : public CName, public base::utils::CTemplatedMapVector< CCinematicPlayer >
{
public:
    CCinematic();
    virtual ~CCinematic();
    bool Load(const CXMLElement* aElement);
    void Update(float elapsedTime);
    void Play();
    GET_SET_BOOL(Active);
    GET_SET_BOOL(Loop);
    GET_SET_BOOL(Reversed);
    GET_SET_BOOL(Finish);
    GET_SET_REF(float, TotalTime);
    GET_SET_REF(float, CurrentTime);
protected:
    bool m_Active;
    bool m_Finish;
    bool m_Loop;
    bool m_Reversed;
    float m_TotalTime;
    bool m_PlayingForward;
    bool m_PlayingBackward;
    float m_CurrentTime;
private:
    DISALLOW_COPY_AND_ASSIGN(CCinematic);
};

#endif