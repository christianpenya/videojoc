

#include <string>
#include "XML/XML.h"
#include "Utils/Logger.h"

#include "Engine/Engine.h"

#include "Physx/PhysxManager.h"

#include "Utils/CheckedRelease.h"
#include "CinematicActor.h"

CCinematicActor::CCinematicActor()

{
}

CCinematicActor::~CCinematicActor()
{
}

void CCinematicActor::Load(CXMLElement* aElement)
{
    cinematicMan = &CEngine::GetInstance().GetCinematicManager();
    cinName = aElement->GetAttribute<std::string>("cinematic_name", "Cinematica01");

}

void CCinematicActor::Act()
{
    cinematicMan->Play(cinName);
}

void CCinematicActor::Update(float elapsedTime)
{

    m_Finished = true;
}
