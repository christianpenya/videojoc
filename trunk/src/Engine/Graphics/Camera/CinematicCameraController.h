#pragma once
#ifndef _ENGINE_CinematicCAMERACONTROLLER_CPB_20161127_H
#define _ENGINE_CinematicCAMERACONTROLLER_CPB_20161127_H

#include "CameraController.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CCinematicCameraController : public CCameraController
{
public:


    CCinematicCameraController(Vect3f initialPos, Vect3f initialUp, Vect3f lFront)
    {
        m_Position = initialPos;
        m_Up = initialUp;
        m_Front = lFront;
    };
    CCinematicCameraController(CCameraController *cam)
    {
        m_Position = cam->GetPosition();
        m_Up = cam->GetUp();
        m_Front = cam->GetFront();
    };
    virtual ~CCinematicCameraController() {};

    virtual void Update(float ElapsedTime) override;


private:

};

#endif //_ENGINE_TPSCAMERACONTROLLER_CPB_20161127_H
