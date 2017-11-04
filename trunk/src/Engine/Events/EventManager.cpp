#include "EventManager.h"

#include "XML/tinyxml2/tinyxml2.h"
#include "XML/XML.h"
#include "Imgui/imgui.h"

#include "Events/DumbActor.h"
#include "Events/AudioTriggerActor.h"

#include "Events/DumbReactor.h"
#include "Events/OpenDoorReactor.h"
#include "Events/TextGUIReactor.h"
#include "Events/TextGUIActor.h"
#include "Events/CheckpointActor.h"

CEventManager::CEventManager() : mEnabled(true)
{
    mActors.Add("dumb", new CDumbActor());
    mActors.Add("audio_trigger", new CAudioTriggerActor());
    mActors.Add("textGUIA", new CTextGUIActor());
    mActors.Add("checkpoint", new CCheckpointActor());

    mReactors.Add("dumb", new CDumbReactor());
    mReactors.Add("open_door", new COpenDoorReactor());
    mReactors.Add("textGUIR", new CTextGUIReactor());
}

CEventManager::~CEventManager()
{
    __H_CHECKED_DELETE__(mFilename);
}

bool CEventManager::Load(std::string aFilename)
{
    mFilename = aFilename;
    return Load();
}

bool CEventManager::Load()
{
    bool lOk = true;

    if (mEnabled)
    {
        CXMLDocument document;
        EXMLParseError error = document.LoadFile((mFilename).c_str());

        if (base::xml::SucceedLoad(error))
        {
            CXMLElement * lEvents = document.FirstChildElement("events");

            if (lEvents)
            {
                for (tinyxml2::XMLElement *iEvent = lEvents->FirstChildElement(); iEvent != nullptr; iEvent = iEvent->NextSiblingElement())
                {
                    if (strcmp(iEvent->Name(), "event") == 0)
                    {
                        CEvent* lEvent = new CEvent(iEvent);
                        lOk &= Add(lEvent->GetName(), lEvent);
                    }
                }
            }
        }

        return lOk;
    }
}

void CEventManager::Update(float elapsedTime)
{
    if (mEnabled)
    {
        for (std::vector<CEvent*>::iterator iEvent = m_ResourcesVector.begin(); iEvent != m_ResourcesVector.end(); ++iEvent)
        {
            if ((*iEvent)->IsHappeningRightFuckingNow())
            {
                (*iEvent)->Update(elapsedTime);
            }
        }
    }
}

CEvent* CEventManager::GetEvent(std::string aEvent)
{
    if (m_ResourcesMap.find(aEvent) != m_ResourcesMap.end())
    {
        return  m_ResourcesMap.find(aEvent)->second.m_Value;
    }
    else
    {
        return nullptr;
    }
}

void CEventManager::DrawImgui()
{
    if (ImGui::TreeNode("Event Manager"))
    {
        for (std::vector<CEvent*>::iterator iEvent = m_ResourcesVector.begin(); iEvent != m_ResourcesVector.end(); ++iEvent)
        {
            ImGui::PushID((*iEvent)->GetName().c_str());

            if (ImGui::CollapsingHeader((*iEvent)->GetName().c_str()))
            {
                //(*iEvent)->DrawImGui();
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
}
