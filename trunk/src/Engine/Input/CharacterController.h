#pragma once
#ifndef _ENGINE_CHARACTERCONTROLLER_20170310_H
#define _ENGINE_CHARACTERCONTROLLER_20170310_H

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif
#include "Math/Vector3.h"

class CCharacterController
{

public:
    CCharacterController() :
        m_Movement(0, 0, 0) {}

    ~CCharacterController() {}

    Vect3f 					m_Movement;
};

#endif //_ENGINE_CAMERACONTROLLER_CPB_20161126_H
