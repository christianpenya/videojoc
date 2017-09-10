#include "Event.h"

#include <string>
#include "XML/XML.h"
#include "Actor.h"
#include "Reactor.h"
#include "Engine/Engine.h"
#include "Events/EventManager.h"
#include "Utils/Logger.h"

CEvent::CEvent() :
    m_Finished(false),
    m_HappeningRightFuckingNow(false),
    mAlreadyReacting(false) {}

CEvent::CEvent(CXMLElement* aEvent) :
    m_Finished(false),
    m_HappeningRightFuckingNow(false),
    mAlreadyReacting(false),
    CName(aEvent->GetAttribute<std::string>("name", ""))
{
    m_Active = aEvent->GetAttribute<bool>("active", false);

    for (tinyxml2::XMLElement *iElement = aEvent->FirstChildElement(); iElement != nullptr; iElement = iElement->NextSiblingElement())
    {
        if (strcmp(iElement->Name(), "actor") == 0)
        {
            std::string lActorName = iElement->GetAttribute<std::string>("name", "");
            mActor = CEngine::GetInstance().GetEventManager().GetActor(lActorName);
            mActor->Load(iElement);
        }
        else if (strcmp(iElement->Name(), "reactor") == 0)
        {
            std::string lReactorName = iElement->GetAttribute<std::string>("name", "");
            mReactor = CEngine::GetInstance().GetEventManager().GetReactor(lReactorName);
            mReactor->Load(iElement);
        }
    }
}

CEvent::~CEvent() {}

void CEvent::Start()
{
    mActor->Act();
    m_HappeningRightFuckingNow = true;
}

void CEvent::Update(float elapsedTime)
{
    if (mActor->IsFinished() && !mAlreadyReacting)
    {
        mReactor->React();
        mAlreadyReacting = true;
    }

    if (!mActor->IsFinished())
    {
        mActor->Update(elapsedTime);
    }
    else if (mActor->IsFinished() && !mReactor->IsFinished())
    {
        mReactor->Update(elapsedTime);
    }
    else if (mActor->IsFinished() && mReactor->IsFinished())
    {
        m_Finished = true;
        m_HappeningRightFuckingNow = false;
    }
}