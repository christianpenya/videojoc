#include "TpsCameraController.h"
#include "Engine/Engine.h"
#include "Physx/PhysxManager.h"
#include "Utils/Logger.h"
#include "Events/LevelController.h"

void CTpsCameraController::Update(float ElapsedTime)
{
    CLevelController *contr = CEngine::GetInstance().m_LevelController;
    if (m_player != nullptr)
    {
        center = m_player->m_Position + playerHeight;
        if (!contr->GetTimePaused())
        {
            yawSpeed = 0.1f * (*actionManager)("pitch")->value;
            pitchSpeed = -0.1f * (*actionManager)("yaw")->value;
        }
        //zoomSpeed = 0.5f*(*actionManager)("zoom")->value;

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


        float l_zoom = zoom;
        Vect3f l_Position = (center - m_Front * l_zoom);
        //Vect3f dir = m_Position - m_player->m_Position;
        Vect3f origin = center - (m_Front * pRadius);
        CPhysXManager::RaycastData *data = new CPhysXManager::RaycastData;
        bool hit = physX->RaycastCam(origin, l_Position, 3, data); //3 | 4, data);
        if (hit)
        {
            LOG_INFO_APPLICATION("HIT \\(^-^)/");
            l_zoom = data->distance - hitOffset;
            if (l_zoom < pRadius)
                l_zoom = -pRadius / 2;
            else
                l_zoom = hitOffset;
            l_Position = data->position + m_Front*l_zoom;
        }
        else
        {
            LOG_INFO_APPLICATION("NOt HIT \\(^-^)/");
        }


        //m_Position = (center - m_Front * l_zoom);
        m_Position = l_Position;
    }
}

void CTpsCameraController::Init(CCharacterController* player)
{
    m_player = player;
    actionManager = &CEngine::GetInstance().GetActionManager();
    physX = &CEngine::GetInstance().GetPhysXManager();
}
