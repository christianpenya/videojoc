#pragma once
#ifndef _ENGINE_CAMERACONTROLLER_CPB_20161126_H
#define _ENGINE_CAMERACONTROLLER_CPB_20161126_H

#include "Render\RenderManager.h"

class CCameraController
{
public:
    CCameraController() :
        m_Position(0, 0, 0),
        m_Front(0, 0, 1),
        m_Up(0, 1, 0) {}

    virtual ~CCameraController() {}
    virtual void Update(float ElapsedTime) = 0;

    void SetToRenderManager(CRenderManager &_RenderManager) const
    {
        _RenderManager.SetViewMatrix(m_Position, m_Position + m_Front, m_Up);
    }

    Vect3f getFront()
    {
        return m_Front;
    }

    Vect3f getUp()
    {
        return m_Up;
    }

    Vect3f getPosition()
    {
        return m_Position;
    }

protected:
    Vect3f 					m_Position;
    Vect3f 					m_Front;
    Vect3f 					m_Up;

    float clamp(float x, float upper, float lower)
    {
        return min(upper, max(x, lower));
    }
};

#endif //_ENGINE_CAMERACONTROLLER_CPB_20161126_H
