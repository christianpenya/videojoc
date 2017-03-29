#include "Cinematic.h"
#include "Utils/CheckedRelease.h"
#include "CinematicPlayer.h"
#include "CinematicCameraPlayer.h"
#include "CinematicObjectPlayer.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "Engine/Engine.h"

CCinematic::CCinematic():
    CName(""),
    m_lastCameraState(nullptr),
    m_Active(false),
    m_Finish(true),
    m_Loop(false),
    m_Reversed(false),
    m_TotalTime(0.0f),
    m_PlayingForward(true),
    m_PlayingBackward(false),
    m_CurrentTime(0.0f)
{}

CCinematic::~CCinematic()
{
    CTemplatedMapVector<CCinematicPlayer>::Destroy();
}

bool CCinematic::Load(const CXMLElement* aElement)
{
    bool lOk = false;
    m_Name = aElement->GetAttribute<std::string>("name", "");
    m_TotalTime = aElement->GetAttribute<float>("total_time", 1.0f);
    m_Loop = aElement->GetAttribute<bool>("loop", false);
    m_Reversed = aElement->GetAttribute<bool>("reverse", false);

    for (tinyxml2::XMLElement const *lCinematic_player = aElement->FirstChildElement(); lCinematic_player != nullptr; lCinematic_player = lCinematic_player->NextSiblingElement())
    {
        CCinematicPlayer *lCinematicPlayer = nullptr;
        bool lOk = true;

        if (strcmp(lCinematic_player->Name(),"cinematic_camera_player")==0)
        {
            lCinematicPlayer = new CCinematicCameraPlayer();
            lOk = lCinematicPlayer->Load(lCinematic_player);
        }
        else if (strcmp(lCinematic_player->Name(), "cinematic_object_player")==0)
        {
            lCinematicPlayer = new CCinematicObjectPlayer();
            lOk = lCinematicPlayer->Load(lCinematic_player);
        }
        if (lCinematicPlayer != nullptr && lOk)
        {
            Add(lCinematicPlayer->mName, lCinematicPlayer);
        }
    }

    return true;
}

void CCinematic::Update(float elapsedTime)
{
    if (m_PlayingForward && !m_Finish)
    {
        m_CurrentTime += elapsedTime;
        if (m_CurrentTime >= m_TotalTime)
        {
            if (m_Reversed)
            {
                m_PlayingBackward = true;
                m_PlayingForward = false;
            }
            else if (m_Loop)
            {
                m_CurrentTime = 0.0f;
            }
            else
            {
                m_Finish = true;
                m_CurrentTime = 0.0f;
                CEngine::GetInstance().SetCameraController(m_lastCameraState);
            }
        }
        else
        {
            for (TMapResources::iterator iPlayerMapEntry = m_ResourcesMap.begin(); iPlayerMapEntry != m_ResourcesMap.end(); ++iPlayerMapEntry)
            {
                CCinematicPlayer* lCinematPlay = iPlayerMapEntry->second.m_Value;
                lCinematPlay->PlayFoward(m_CurrentTime);
            }
        }

    }
    else if (m_PlayingBackward && !m_Finish)
    {
        m_CurrentTime -= elapsedTime;
        for (TMapResources::iterator iPlayerMapEntry = m_ResourcesMap.begin(); iPlayerMapEntry != m_ResourcesMap.end(); ++iPlayerMapEntry)
        {
            CCinematicPlayer* lCinematPlay = iPlayerMapEntry->second.m_Value;
            lCinematPlay->PlayFoward(m_CurrentTime);

        }
        if (m_CurrentTime <= 0.0f)
        {
            if (m_Loop && m_Reversed)
            {
                m_CurrentTime = 0.0f;
                m_PlayingForward = true;
                m_PlayingBackward = false;
            }
            else
            {
                m_Finish = true;
                m_CurrentTime = m_TotalTime;
            }
        }

    }

}

void CCinematic::Play()
{
    m_lastCameraState = &CEngine::GetInstance().GetCameraController();
    m_CurrentTime = 0.0f;
    m_Finish = false;
    m_Active = true;
}