#pragma once
#ifndef _ENGINE_CINEMATIC_CAMERA_PLAYER_20170213
#define _ENGINE_CINEMATIC_CAMERA_PLAYER_20170213

#include "CinematicPlayer.h"

class CCameraController;
class CCinematicCameraPlayer : public CCinematicPlayer
{
public:
    CCinematicCameraPlayer();
    virtual ~CCinematicCameraPlayer();
    virtual bool Load(const CXMLElement* aElement);
    virtual void Finish();
    virtual void Start();
private:
    DISALLOW_COPY_AND_ASSIGN(CCinematicCameraPlayer);
protected:
    virtual void Apply(float aPercentage, CCinematicKey* A, CCinematicKey* B);
    CCameraController *m_lastCameraState;
    CCameraController *m_CurrentCinematicCamera;
};

#endif