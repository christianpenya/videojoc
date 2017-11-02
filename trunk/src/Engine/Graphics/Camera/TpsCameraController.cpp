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
        else
        {
            yawSpeed = 0.f;
            pitchSpeed = 0.f;
        }
        //zoomSpeed = 0.5f*(*actionManager)("zoom")->value;

        m_Yaw += yawSpeed * ElapsedTime;
        m_Pitch += pitchSpeed * ElapsedTime;
        m_Roll += rollSpeed * ElapsedTime;
        zoom += zoomSpeed * ElapsedTime;

        if (m_Pitch > maxPitch)
            m_Pitch = maxPitch;
        if (m_Pitch < minPitch)
            m_Pitch = minPitch;

        if (zoom > maxZoom)
            zoom = maxZoom;
        if (zoom < minZoom)
            zoom = minZoom;

        if (m_Yaw > mathUtils::PiTimes<float>())
            m_Yaw -= mathUtils::PiTimes<float>(2.0f);
        if (m_Yaw < -mathUtils::PiTimes<float>())
            m_Yaw += mathUtils::PiTimes<float>(2.0f);

        if (m_Roll > mathUtils::PiTimes<float>())
            m_Roll -= mathUtils::PiTimes<float>(2.0f);
        if (m_Roll < -mathUtils::PiTimes<float>())
            m_Roll += mathUtils::PiTimes<float>(2.0f);

        // set CameraControllerData
        m_Front.x = sin(m_Yaw) * cos(-m_Pitch);
        m_Front.y = -sin(-m_Pitch);
        m_Front.z = -cos(m_Yaw) * cos(-m_Pitch);

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
