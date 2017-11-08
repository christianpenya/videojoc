#include "XML/XML.h"

#include "Engine/Engine.h"

#include "WaitEActor.h"

CWaitEActor::CWaitEActor() : pressed(false)
{
}

CWaitEActor::~CWaitEActor()
{
}

void CWaitEActor::Load(CXMLElement* aElement)
{

    ActionMan = &CEngine::GetInstance().GetActionManager();

}

void CWaitEActor::Act()
{
    mElapsed = 0;
}

void CWaitEActor::Update(float elapsedTime)
{
    mElapsed += elapsedTime;
    if ((*ActionMan)("use")->value>0.1)
        pressed = true;
    if (pressed==true && mElapsed>1.f)
        m_Finished = true;
}
