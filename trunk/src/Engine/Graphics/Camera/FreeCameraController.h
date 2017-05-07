#pragma once
#ifndef _ENGINE_FREECAMERACONTROLLER_CPB_20170507_H
#define _ENGINE_FREECAMERACONTROLLER_CPB_20170507_H

#include "CameraController.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CFreeCameraController : public CCameraController
{
public:
    Vect3f center;
    float xSpeed, ySpeed, zSpeed;
    float yawSpeed, pitchSpeed, rollSpeed;
    float zoomSpeed;

    CFreeCameraController(Vect3f position = Vect3f(0, 10, 0), Vect3f forward = Vect3f(0, 0, 1), Vect3f up = Vect3f(0, 1, 0),
                          Vect4f cameraInfo = Vect4f(1, 500, 1.13f, 1.7f), float maxPitch = 1.5f, float minPitch = -1.5f, float maxZoom = 10.0f, float minZoom = -10.0f)
        : CCameraController(position, forward, up, cameraInfo)
        , center(Vect3f(0, 0, 0))
        , maxPitch(maxPitch)
        , minPitch(minPitch)
        , maxZoom(maxZoom)
        , minZoom(minZoom)
        , xSpeed(0.0f)
        , ySpeed(0.0f)
        , zSpeed(0.0f)
        , yawSpeed(0.0f)
        , pitchSpeed(0.0f)
        , rollSpeed(0.0f)
        , zoomSpeed(0.0f)
        , yaw(0.0f)
        , pitch(0.0f)
        , roll(0.0f)
        , maxDistance(5.0f)
    {};

    virtual ~CFreeCameraController() {};
    virtual void Update(float ElapsedTime) override;

private:

    float yaw, pitch, roll, zoom;
    float maxPitch, minPitch;
    float maxZoom, minZoom;
    float maxDistance;
};

#endif //_ENGINE_FREECAMERACONTROLLER_CPB_20170507_H
