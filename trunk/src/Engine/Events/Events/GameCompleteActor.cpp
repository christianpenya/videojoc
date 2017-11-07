#include "DumbActor.h"

#include <string>
#include "XML/XML.h"
#include "Utils/Logger.h"

#include "Engine/Engine.h"
#include "GameCompleteActor.h"
#include "Events/LevelController.h"

CGameCompleteActor::CGameCompleteActor() {}
CGameCompleteActor::~CGameCompleteActor() {}
void CGameCompleteActor::Load(CXMLElement* aElement)
{




}

void CGameCompleteActor::Act()
{
    CEngine::GetInstance().m_LevelController->GameComplete();

}

void CGameCompleteActor::Update(float elapsedTime)
{
    m_Finished = true;

}