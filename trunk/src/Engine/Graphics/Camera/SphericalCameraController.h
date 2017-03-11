#pragma once
#ifndef _ENGINE_SPHERICALCAMERACONTROLLER_CPB_20161126_H
#define _ENGINE_SPHERICALCAMERACONTROLLER_CPB_20161126_H

#include "CameraController.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CSphericalCameraController : public CCameraController
{
public:
    Vect3f center;
    float yawSpeed, pitchSpeed, rollSpeed;
    float zoomSpeed;

    CSphericalCameraController(Vect3f center = Vect3f(0, 0, 0), float maxPitch = 1.5f, float minPitch = -1.5f, float maxZoom = 10, float minZoom = 0)
        : center(center)
        , maxPitch(maxPitch)
        , minPitch(minPitch)
        , maxZoom(maxZoom)
        , minZoom(minZoom)
        , yawSpeed(0)
        , pitchSpeed(0)
        , rollSpeed(0)
        , zoomSpeed(0)
        , yaw(1.0f)
        , pitch(-1.0f)
        , roll(0.0)
        , zoom(10.0)
    {};
    virtual ~CSphericalCameraController() {};

    virtual void Update(float ElapsedTime) override;

private:
    float yaw, pitch, roll, zoom;

    float maxPitch, minPitch;
    float maxZoom, minZoom;
};

#endif //_ENGINE_SPHERICALCAMERACONTROLLER_CPB_20161126_H
