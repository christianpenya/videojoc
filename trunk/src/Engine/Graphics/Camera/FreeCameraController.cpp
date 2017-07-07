#include "FreeCameraController.h"
#include "Input/ActionManager.h"
#include "Engine/Engine.h"
#include "Utils/Logger.h"
#include "ImGUI\imgui.h"

void CFreeCameraController::Update(float ElapsedTime)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;


    CCameraController::Update(ElapsedTime);
    CActionManager* actionManager = &CEngine::GetInstance().GetActionManager();
    center = CEngine::GetInstance().GetRenderManager().m_SphereOffset;

    if ((*actionManager)("mouse_right_pressed")->value<0.99)
        return;

    xSpeed = 0.2f * (*actionManager)("x_move")->value;
    zSpeed = 0.1f * (*actionManager)("z_move")->value;
    yawSpeed = 0.1f * (*actionManager)("pitch")->value;
    pitchSpeed = -0.1f * (*actionManager)("yaw")->value;
    zoomSpeed = 0.1f * (*actionManager)("zoom")->value;

    yaw += yawSpeed * ElapsedTime;
    pitch += pitchSpeed * ElapsedTime;
    roll += rollSpeed * ElapsedTime;
    zoom = zoomSpeed * ElapsedTime;

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

    m_Front.x = sin(yaw) * cos(-pitch);
    m_Front.y = -sin(-pitch);
    m_Front.z = -cos(yaw) * cos(-pitch);

    m_Position += zSpeed * m_Front;

    Vect3f right = m_Front ^ m_Up;
    m_Position += xSpeed * right;
    m_Position += m_Front * zoom;
}
