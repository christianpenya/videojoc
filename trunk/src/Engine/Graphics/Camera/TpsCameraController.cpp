#include "TpsCameraController.h"
#include "Engine/Engine.h"

void CTpsCameraController::Update(float ElapsedTime)
{
    center = m_player->m_Position;
    yawSpeed = 0.1f * (*actionManager)("pitch")->value;
    pitchSpeed = -0.1f * (*actionManager)("yaw")->value;

    zoomSpeed = (*actionManager)("zoom")->value;

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

void CTpsCameraController::Init(CCharacterController* player)
{

    m_player = player;
    actionManager = &CEngine::GetInstance().GetActionManager();
}
