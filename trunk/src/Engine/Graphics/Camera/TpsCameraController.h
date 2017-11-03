#pragma once
#ifndef _ENGINE_TPSCAMERACONTROLLER_CPB_20161127_H
#define _ENGINE_TPSCAMERACONTROLLER_CPB_20161127_H

#include "CameraController.h"
#include "Input/ActionManager.h"
#include "Input/CharacterController.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CTpsCameraController : public CCameraController
{
public:
    Vect3f center;
    float yawSpeed, pitchSpeed, rollSpeed;
    float zoomSpeed;
    CActionManager* actionManager;
    CCharacterController* m_player;
    CPhysXManager* physX;
    GET_SET(float, Pitch)
    GET_SET(float, Yaw)
    GET_SET(float, Roll)
    CTpsCameraController( Vect3f center = Vect3f(0, 0, 0), float maxPitch = 0.4f, float minPitch = -0.5f, float maxZoom = 20.0f, float minZoom = 0.5f)
        : center(center)
        , maxPitch(maxPitch)
        , minPitch(minPitch)
        , maxZoom(maxZoom)
        , minZoom(minZoom)
        , yawSpeed(0.0f)
        , pitchSpeed(0.0f)
        , rollSpeed(0.0f)
        , zoomSpeed(0.0f)
        , m_Yaw(3.14f)
        , m_Pitch(0.0f)
        , m_Roll(0.0f)
        , zoom(3.0f)
        , hitOffset(0.3f)
        , pRadius(0.15f)
    {

    };
    virtual ~CTpsCameraController() {};

    virtual void Update(float ElapsedTime) override;
    virtual void Init(CCharacterController* player);


private:
    Vect3f playerHeight = Vect3f(0, 1.7f, 0);
    float m_Yaw, m_Pitch, m_Roll, zoom;

    float maxPitch, minPitch;
    float maxZoom, minZoom;
    float hitOffset;
    float pRadius;



};

#endif //_ENGINE_TPSCAMERACONTROLLER_CPB_20161127_H
