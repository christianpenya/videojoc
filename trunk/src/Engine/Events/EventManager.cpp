#include "EventManager.h"

#include "XML/tinyxml2/tinyxml2.h"
#include "XML/XML.h"
#include "Imgui/imgui.h"

#include "Events/DumbActor.h"
#include "Events/DumbReactor.h"
#include "Events/OpenDoorReactor.h"

CEventManager::CEventManager()
{
    mActors.Add("dumb", new CDumbActor());

    mReactors.Add("dumb", new CDumbReactor());
    mReactors.Add("open_door", new COpenDoorReactor());
}

CEventManager::~CEventManager()
{
    __H_CHECKED_DELETE__(mActors);
    __H_CHECKED_DELETE__(mReactors);
}

bool CEventManager::Load(std::string aFilename)
{
    mFilename = aFilename;
    return Load();
}

bool CEventManager::Load()
{
    bool lOk = true;

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

void CEventManager::Update(float elapsedTime)
{
    for (std::vector<CEvent*>::iterator iEvent = m_ResourcesVector.begin(); iEvent != m_ResourcesVector.end(); ++iEvent)
    {
        if ((*iEvent)->mIsEnCours)
        {
            (*iEvent)->Update(elapsedTime);
        }
    }
}

CEvent* CEventManager::GetEvent(std::string aEvent)
{
    return  m_ResourcesMap.find(aEvent)->second.m_Value;
}

CActor* CEventManager::GetActor(std::string aActor)
{
    return mActors(aActor);
}

CReactor* CEventManager::GetReactor(std::string aReactor)
{
    return mReactors(aReactor);
}
