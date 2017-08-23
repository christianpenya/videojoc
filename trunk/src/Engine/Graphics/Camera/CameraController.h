#pragma once
#ifndef _ENGINE_CAMERACONTROLLER_CPB_20161126_H
#define _ENGINE_CAMERACONTROLLER_CPB_20161126_H

#include "Render\RenderManager.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CCameraController
{
public:
    CCameraController() :
        m_Position(0, 0, 0),
        m_Front(0, 0, 1),
        m_Up(0, 1, 0),
        m_CameraInfo(0.01f, 500, 1.13f, 1.7f) {} //	m_CameraInfo x = near, y = far, z = fov, w = aspect ratio

    CCameraController(Vect3f position, Vect3f front, Vect3f up, Vect4f cameraInfo) :
        m_Position(position),
        m_Front(front),
        m_Up(up),
        m_CameraInfo(cameraInfo) {}

    virtual ~CCameraController() {}
    virtual void Update(float ElapsedTime) = 0;

    void SetToRenderManager(CRenderManager &_RenderManager) const
    {
        _RenderManager.SetViewMatrix(m_Position, m_Position + m_Front, m_Up);
        _RenderManager.SetProjectionMatrix(m_CameraInfo.z, m_CameraInfo.w, 0.01f, m_CameraInfo.y);
    }

    Vect4f GetPositionInScreenCoordinates(const Vect3f &Position) const;
    Vect3f GetFront()
    {
        return m_Front;
    }

    Vect3f GetUp()
    {
        return m_Up;
    }

    Vect3f GetPosition()
    {
        return m_Position;
    }

    Vect4f GetCameraInfo()
    {
        return m_CameraInfo;
    }

    void SetFront(Vect3f front)
    {
        m_Front = front;
    }

    void SetUp(Vect3f up)
    {
        m_Up = up;
    }

    void SetPosition(Vect3f pos)
    {
        m_Position = pos;
    }

    void SetCameraInfo(Vect4f info)
    {
        m_CameraInfo = info;
    }

protected:
    Vect3f 					m_Position;
    Vect3f 					m_Front;
    Vect3f 					m_Up;
    Vect4f 					m_CameraInfo;

    float Clamp(float x, float upper, float lower)
    {
        return min(upper, max(x, lower));
    }
};

#endif //_ENGINE_CAMERACONTROLLER_CPB_20161126_H

