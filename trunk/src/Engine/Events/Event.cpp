#include "Event.h"

#include <string>
#include "XML/XML.h"
#include "Actor.h"
#include "Reactor.h"
#include "Engine/Engine.h"
#include "Events/EventManager.h"
#include "Utils/Logger.h"

CEvent::CEvent() :
    mIsEnCours(false),
    mAlreadyReacting(false) {}

CEvent::CEvent(CXMLElement* aEvent) :
    mIsEnCours(false),
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
        }
        else if (strcmp(iElement->Name(), "reactor") == 0)
        {
            std::string lReactorName = iElement->GetAttribute<std::string>("name", "");
            mReactor = CEngine::GetInstance().GetEventManager().GetReactor(lReactorName);
        }
    }
}

CEvent::~CEvent() {}

void CEvent::Start()
{
    mIsEnCours = true;
    mActor->Act();
}

void CEvent::Update()
{
    if (mActor->IsFinished() && !mAlreadyReacting)
    {
        mReactor->React();
        mAlreadyReacting = true;
    }

    if (!mActor->IsFinished())
    {
        mActor->Update();
    }
    else if (mActor->IsFinished() && !mReactor->IsFinished())
    {
        mReactor->Update();
    }
    else if (mActor->IsFinished() && mReactor->IsFinished())
    {
        mIsEnCours = false;
    }
}