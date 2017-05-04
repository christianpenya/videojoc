#include "SphericalCameraController.h"
#include "Input/ActionManager.h"
#include "Engine/Engine.h"

void CSphericalCameraController::Update(float ElapsedTime)
{
    CActionManager* actionManager = &CEngine::GetInstance().GetActionManager();
    zoomSpeed = (*actionManager)("zoom")->value;

    if ((*actionManager)("enable_rotation")->active)
    {
        yawSpeed = 0.1f * (*actionManager)("pitch")->value;
        pitchSpeed = -0.1f * (*actionManager)("yaw")->value;
    }
    else
    {
        yawSpeed = 0.0;
        pitchSpeed = 0.0;
    }
    yaw += yawSpeed * ElapsedTime;
    pitch += pitchSpeed * ElapsedTime;
    roll += rollSpeed * ElapsedTime;
    zoom += zoomSpeed * ElapsedTime;

    if (pitch > maxPitch)
        pitch = maxPitch;
    if (pitch < minPitch)
        pitch = minPitch;

    if (zoom > maxZoom)
        zoom = maxZoom;
    if (zoom < minZoom)
        zoom = minZoom;

    if (yaw > mathUtils::PiTimes<float>())
        yaw -= mathUtils::PiTimes<float>(2.0f);
    if (yaw < -mathUtils::PiTimes<float>())
        yaw += mathUtils::PiTimes<float>(2.0f);

    if (roll > mathUtils::PiTimes<float>())
        roll -= mathUtils::PiTimes<float>(2.0f);
    if (roll < -mathUtils::PiTimes<float>())
        roll += mathUtils::PiTimes<float>(2.0f);

    // set CameraControllerData
    m_Front.x = sin(yaw) * cos(-pitch);
    m_Front.y = -sin(-pitch);
    m_Front.z = -cos(yaw) * cos(-pitch);

    // TODO roll

    m_Position = center - m_Front * zoom;

}
