#pragma once
#ifndef _ENGINE_CINEMATIC_CAMERA_PLAYER_20170213
#define _ENGINE_CINEMATIC_CAMERA_PLAYER_20170213

#include "CinematicPlayer.h"
#include "Engine\Engine.h"
#include "Camera\CameraController.h"
#include "CinematicCameraKey.h"



class CCinematicCameraPlayer : public CCinematicPlayer
{
public:
    CCinematicCameraPlayer();
    virtual ~CCinematicCameraPlayer();
    virtual bool Load(const CXMLElement* aElement);
private:
    DISALLOW_COPY_AND_ASSIGN(CCinematicCameraPlayer);
protected:
    virtual void Apply(float aPercentage, CCinematicKey* A, CCinematicKey* B);
};

#endif