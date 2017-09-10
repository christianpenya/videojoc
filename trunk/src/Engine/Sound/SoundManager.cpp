#include "Sound/SoundManager.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "XML/XML.h"
#include "Graphics/Camera/CameraController.h"
#include "Utils/Logger.h"
#include "Imgui/imgui.h"

CSoundManager::CSoundManager()
{
    m_LastGameObjectID = 1;
}
CSoundManager::~CSoundManager()
{
    Terminate();
    m_FreeObjectsIDs.clear();
    m_NamedSpeakers.clear();
    m_GameObjectSpeakers.clear();
}

ISoundManager* ISoundManager::InstantiateSoundManager()
{
    return new CSoundManager();
}

void CSoundManager::SetPath(const std::string &path)
{
    m_Path = path;
}

bool CSoundManager::Init()
{
    AkMemSettings memSettings;
    memSettings.uMaxNumPools = 20;

    AkStreamMgrSettings stmSettings;
    AK::StreamMgr::GetDefaultSettings(stmSettings);

    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

    AkInitSettings l_InitSettings;
    AkPlatformInitSettings l_platInitSettings;
    AK::SoundEngine::GetDefaultInitSettings(l_InitSettings);
    AK::SoundEngine::GetDefaultPlatformInitSettings(l_platInitSettings);

    l_InitSettings.uDefaultPoolSize = 512 * 1024;
    l_InitSettings.uMaxNumPaths = 16;
    l_InitSettings.uMaxNumTransitions = 128;

    l_platInitSettings.uLEngineDefaultPoolSize = 512 * 1024;

    AkMusicSettings musicInit;
    AK::MusicEngine::GetDefaultInitSettings(musicInit);

    AKRESULT eResult = AK::SOUNDENGINE_DLL::Init(&memSettings, &stmSettings, &deviceSettings, &l_InitSettings, &l_platInitSettings, &musicInit);

    if (eResult != AK_Success)
    {
        AK::SOUNDENGINE_DLL::Term();
        LOG_ERROR_APPLICATION("Error initializing CSoundManager");

        return false;

    }

    m_DefaultSpeakerId = GenerateObjectID();
    AK::SoundEngine::RegisterGameObj(m_DefaultSpeakerId);
}

void CSoundManager::Update(CCameraController *camera)
{
    // Update dynamic speaker position
    for (auto it : m_GameObjectSpeakers)
    {
        Vect3f l_SpeakerPosition = it.first->GetPosition();
        Vect3f l_SpeakerForward = it.first->GetForward();

        AkSoundPosition l_SoundPosition = {};

        l_SoundPosition.Position.X = -l_SpeakerPosition.x;
        l_SoundPosition.Position.Y = l_SpeakerPosition.y;
        l_SoundPosition.Position.Z = l_SpeakerPosition.z;

        l_SoundPosition.Orientation.X = -l_SpeakerForward.x;
        l_SoundPosition.Orientation.Y = l_SpeakerForward.y;
        l_SoundPosition.Orientation.Z = l_SpeakerForward.z;

        AK::SoundEngine::SetPosition(it.second, l_SoundPosition);
    }

    //Update listener
    SetListenerPosition(camera);

    //Update wwise
    AK::SOUNDENGINE_DLL::Tick();
}

void CSoundManager::SetListenerPosition(CCameraController *camera)
{
    Vect3f l_Position = camera->GetPosition();
    Vect3f l_Orientation = camera->GetFront();
    Vect3f l_VectorUp = camera->GetUp();

    AkListenerPosition l_ListenerPosition = {};

    l_ListenerPosition.Position.X = -l_Position.x;
    l_ListenerPosition.Position.Y = l_Position.y;
    l_ListenerPosition.Position.Z = l_Position.z;

    l_ListenerPosition.OrientationFront.X = -l_Orientation.x;
    l_ListenerPosition.OrientationFront.Y = l_Orientation.y;
    l_ListenerPosition.OrientationFront.Z = l_Orientation.z;

    l_ListenerPosition.OrientationTop.X = -l_VectorUp.x;
    l_ListenerPosition.OrientationTop.Y = l_VectorUp.y;
    l_ListenerPosition.OrientationTop.Z = l_VectorUp.z;

    AK::SoundEngine::SetListenerPosition(l_ListenerPosition);
}

bool CSoundManager::Load(const std::string &soundbanks_filename, const std::string &speakers_filename)
{
    m_SoundBanksFilename = soundbanks_filename;
    m_SpeakersFilename = speakers_filename;

    bool l_IsOk = true;
    l_IsOk = LoadSoundBanksXML();
    l_IsOk &= LoadSpeakersXML();

    return l_IsOk;
}

bool CSoundManager::Reload()
{
    return false;
}

bool CSoundManager::LoadSoundBank(const std::string &bank)
{
    AkBankID bankID;
    AKRESULT retValue;

    retValue = AK::SoundEngine::LoadBank(bank.c_str(), AK_DEFAULT_POOL_ID, bankID);

    if (retValue != AK_Success)
    {
        return false;
    }

    return true;
}

bool CSoundManager::UnloadSoundBank(const std::string &bank)
{
    AKRESULT retValue;

    retValue = AK::SoundEngine::UnloadBank(bank.c_str(), nullptr);
    if (retValue != AK_Success)
    {
        return false;
    }

    return true;
}

void CSoundManager::RegisterSpeaker(CSceneNode* _speaker)
{
    //assert(m_GameObjectSpeakers.find(_speaker) == m_GameObjectSpeakers.end());
    if (m_GameObjectSpeakers.find(_speaker) != m_GameObjectSpeakers.end())
        UnregisterSpeaker(_speaker);

    AkGameObjectID id = GenerateObjectID();
    m_GameObjectSpeakers[_speaker] = id;

    Vect3f l_Position = _speaker->GetPosition();
    Vect3f l_Orientation = _speaker->GetForward();

    AkSoundPosition l_SoundPosition = {};

    l_SoundPosition.Position.X = -l_Position.x;
    l_SoundPosition.Position.Y = l_Position.y;
    l_SoundPosition.Position.Z = l_Position.z;

    l_SoundPosition.Orientation.X = -l_Orientation.x;
    l_SoundPosition.Orientation.Y = l_Orientation.y;
    l_SoundPosition.Orientation.Z = l_Orientation.z;

    AK::SoundEngine::RegisterGameObj(id);
    AK::SoundEngine::SetPosition(id, l_SoundPosition);
}

void CSoundManager::UnregisterSpeaker(const CSceneNode* _speaker)
{
    auto it = m_GameObjectSpeakers.find(_speaker);
    if (it != m_GameObjectSpeakers.end())
    {
        AK::SoundEngine::UnregisterGameObj(it->second);
        m_FreeObjectsIDs.push_back(it->second);
        m_GameObjectSpeakers.erase(it);
    }
    else
    {
        assert(false);
    }
}

void CSoundManager::PlayEvent(SoundEvent &_event, const AkGameObjectID &id)
{
    AK::SoundEngine::PostEvent(_event.eventName.c_str(), id);
}

void CSoundManager::PlayEvent(SoundEvent &_event)
{
    PlayEvent(_event, m_DefaultSpeakerId);
}

void CSoundManager::PlayEvent(SoundEvent &_event, const std::string &_speaker)
{
    auto it = m_NamedSpeakers.find(_speaker);
    if (it != m_NamedSpeakers.end())
    {
        PlayEvent(_event, it->second);
    }
    else
    {
        assert(false);
    }
}

void CSoundManager::PlayEvent(SoundEvent &_event, const CSceneNode* _speaker)
{
    auto it = m_GameObjectSpeakers.find(_speaker);
    if (it != m_GameObjectSpeakers.end())
    {
        PlayEvent(_event, it->second);
    }
    else
    {
        assert(false);
    }
}
void CSoundManager::SetSwitch(SoundSwitchValue &switchValue, const AkGameObjectID &id)
{
    AKRESULT res = AK::SoundEngine::SetSwitch(switchValue.soundSwitch.switchName.c_str(), switchValue.valueName.c_str(), id);
    assert(res);
}

void CSoundManager::SetSwitch(SoundSwitchValue &switchValue)
{
    SetSwitch(switchValue, m_DefaultSpeakerId);
}

void CSoundManager::SetSwitch(SoundSwitchValue &switchValue, const std::string &_speaker)
{
    auto it = m_NamedSpeakers.find(_speaker);
    if (it != m_NamedSpeakers.end())
    {
        SetSwitch(switchValue, it->second);
    }
    else
    {
        assert(false);
    }
}

void CSoundManager::SetSwitch(SoundSwitchValue &switchValue, const CSceneNode* _speaker)
{
    auto it = m_GameObjectSpeakers.find(_speaker);
    if (it != m_GameObjectSpeakers.end())
    {
        SetSwitch(switchValue, it->second);
    }
    else
    {
        assert(false);
    }
}

void CSoundManager::SetRTPCValue(SoundRTPC &_rtpc, float value, const AkGameObjectID &id)
{
    AKRESULT res = AK::SoundEngine::SetRTPCValue(_rtpc.RTPCName.c_str(), (AkRtpcValue)value, id);
    assert(res);
}

void CSoundManager::SetRTPCValue(SoundRTPC &_rtpc, float value)
{
    SetRTPCValue(_rtpc, value, m_DefaultSpeakerId);
}

void CSoundManager::SetRTPCValue(SoundRTPC &_rtpc, float value, const std::string &_speaker)
{
    auto it = m_NamedSpeakers.find(_speaker);
    if (it != m_NamedSpeakers.end())
    {
        SetRTPCValue(_rtpc, value, it->second);
    }
    else
    {
        assert(false);
    }
}

void CSoundManager::SetRTPCValue(SoundRTPC &_rtpc, float value, const CSceneNode* _speaker)
{
    auto it = m_GameObjectSpeakers.find(_speaker);
    if (it != m_GameObjectSpeakers.end())
    {
        SetRTPCValue(_rtpc, value, it->second);
    }
    else
    {
        assert(false);
    }
}

void CSoundManager::BroadcastRTPCValue(const SoundRTPC &_rtpc, float value)
{
    AKRESULT res = AK::SoundEngine::SetRTPCValue(_rtpc.RTPCName.c_str(), (AkRtpcValue)value);
    assert(res);
}

void CSoundManager::BroadcastState(const SoundStateValue &_state)
{
    AKRESULT res = AK::SoundEngine::SetState(_state.soundState.stateName.c_str(), _state.valueName.c_str());
    assert(res);
}

AkGameObjectID CSoundManager::GenerateObjectID()
{
    AkGameObjectID result;
    if (m_FreeObjectsIDs.size() > 0)
    {
        result = m_FreeObjectsIDs.back();
        m_FreeObjectsIDs.pop_back();
    }
    else
    {
        result = ++m_LastGameObjectID;
    }
    return result;
}

void CSoundManager::Terminate()
{
    AK::SoundEngine::ClearBanks();
    AK::SoundEngine::UnregisterAllGameObj();

    AK::SOUNDENGINE_DLL::Term();
}

void CSoundManager::Clean()
{
    AK::SoundEngine::ClearBanks();

    for (auto it : m_NamedSpeakers)
    {
        AK::SoundEngine::UnregisterGameObj(it.second);
        m_FreeObjectsIDs.push_back(it.second);
    }

    m_NamedSpeakers.clear();
}

bool CSoundManager::LoadSoundBanksXML()
{
    InitBanks();

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError l_Error = doc.LoadFile((m_Path + m_SoundBanksFilename).c_str());

    if (l_Error == tinyxml2::XML_SUCCESS)
    {
        tinyxml2::XMLElement *l_SoundBanksInfo = doc.FirstChildElement("SoundBanksInfo");
        if (l_SoundBanksInfo)
        {
            tinyxml2::XMLElement*  l_SoundBanks = l_SoundBanksInfo->FirstChildElement("SoundBanks");
            if (l_SoundBanks)
            {
                for (tinyxml2::XMLElement* iSoundBank = l_SoundBanks->FirstChildElement("SoundBank"); iSoundBank != nullptr; iSoundBank = iSoundBank->NextSiblingElement("SoundBank"))
                {
                    tinyxml2::XMLElement* l_Path = iSoundBank->FirstChildElement("Path");
                    std::string lName = l_Path->FirstChild()->Value();
                    if (strcmp(l_Path->FirstChild()->Value(), "Init.bnk"))
                    {
                        LoadSoundBank(lName);
                    }
                }
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool CSoundManager::LoadSpeakersXML()
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError l_Error = doc.LoadFile((m_Path + m_SpeakersFilename).c_str());

    tinyxml2::XMLElement* l_Element;

    if (l_Error == tinyxml2::XML_SUCCESS)
    {
        l_Element = doc.FirstChildElement("Speakers")->FirstChildElement();

        while (l_Element != NULL)
        {
            std::string l_Name = l_Element->GetAttribute<std::string>("name", "");;
            Vect3f l_Position = l_Element->GetAttribute<Vect3f>("position", Vect3f(0.0f, 0.0f, 0.0f));
            Vect3f l_Orientation = l_Element->GetAttribute<Vect3f>("forward", Vect3f(1.0f, 1.0f, 1.0f));

            AkSoundPosition l_SoundPosition = {};

            l_SoundPosition.Position.X = -l_Position.x;
            l_SoundPosition.Position.Y = l_Position.y;
            l_SoundPosition.Position.Z = l_Position.z;

            l_SoundPosition.Orientation.X = -l_Orientation.x;
            l_SoundPosition.Orientation.Y = l_Orientation.y;
            l_SoundPosition.Orientation.Z = l_Orientation.z;

            AkGameObjectID id = GenerateObjectID();
            m_NamedSpeakers[l_Name] = id;
            AK::SoundEngine::RegisterGameObj(id);
            AK::SoundEngine::SetPosition(id, l_SoundPosition);
            l_Element = l_Element->NextSiblingElement();
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool CSoundManager::InitBanks()
{
    // load initialization and main soundbanks
    AkOSChar *path;
    AKRESULT retValue;
    CONVERT_CHAR_TO_OSCHAR(m_Path.c_str(), path);
    retValue = AK::SOUNDENGINE_DLL::SetBasePath(path);
    retValue = AK::StreamMgr::SetCurrentLanguage(L"English(US)");

    AkBankID bankID;
    retValue = AK::SoundEngine::LoadBank("Init.bnk", AK_DEFAULT_POOL_ID, bankID);
    if (retValue != AK_Success)
    {
        return false;
    }

    return true;
}

void CSoundManager::OnEventEnd()
{

}

void CSoundManager::DrawImgui()
{
    if (ImGui::CollapsingHeader("Sound Manager"))
    {
        ImGui::BeginChild("#Sounds", ImVec2(400, 200), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::PushItemWidth(-130);

        if (ImGui::Button("Dentro evento!"))
        {
            SoundEvent se;
            se.eventName = "player_footstep";
            PlayEvent(se);
        }

        SoundSwitchValue lSSV;
        lSSV.soundSwitch.switchName = "Speed";
        static int speed = 0;

        ImGui::RadioButton("Walk", &speed, 0);
        ImGui::RadioButton("Run", &speed, 1);

        switch (speed)
        {
        case 0:
            lSSV.valueName = "Walk";
            break;
        case 1:
            lSSV.valueName = "Run";
            break;
        }

        SetSwitch(lSSV);

        SoundRTPC lSRTPC;
        lSRTPC.RTPCName = "music_volume";
        static float lVolume = 0.0f;
        float lLastVolume = lVolume;
        ImGui::SliderFloat("Volume", &lVolume, -100.0f, 100.0f);
        if (lLastVolume != lVolume)
        {
            SetRTPCValue(lSRTPC, lVolume);
        }

        static int lMusicState = 0;
        int lLastMusicState = lMusicState;
        ImGui::RadioButton("Music ON (state)", &lMusicState, 0);
        ImGui::RadioButton("Music OFF (state)", &lMusicState, 1);

        if (lLastMusicState != lMusicState)
        {
            SoundStateValue lSSV;
            lSSV.soundState.stateName = "music_enabling";
            switch (lMusicState)
            {
            case 0:
                lSSV.valueName = "on";
                break;
            case 1:
                lSSV.valueName = "off";
                break;
            }

            BroadcastState(lSSV);
        }

        ImGui::PopItemWidth();
        ImGui::EndChild();
    }
}