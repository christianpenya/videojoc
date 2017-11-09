#include "XML/XML.h"

#include "Engine/Engine.h"

#include "NodeInteractActor.h"

CNodeInteractActor::CNodeInteractActor(): GUIMan(nullptr), RenderMan(nullptr), mMaxTextTime(0), mElapsed(0), pressed(false)
{
}

CNodeInteractActor::~CNodeInteractActor()
{
}

void CNodeInteractActor::Load(CXMLElement* aElement)
{
    GUIMan = &CEngine::GetInstance().GetGUIManager();
    RenderMan = &CEngine::GetInstance().GetRenderManager();
    ActionMan = &CEngine::GetInstance().GetActionManager();
    mText = aElement->GetAttribute<std::string>("text", "Text Not Found :S");
    mFont = aElement->GetAttribute<std::string>("font", "font1");
    mPortion = aElement->GetAttribute<Vect2f>("portion", Vect2f(0, 0));
    mMaxTextTime = aElement->GetAttribute<float>("time", 5.f);
}

void CNodeInteractActor::Act()
{
    mElapsed = 0;

}

void CNodeInteractActor::Update(float elapsedTime)
{
    mElapsed += elapsedTime;
    Vect2u lSize = RenderMan->GetWindowSize();
    Vect2f lPos;
    lPos.x = lSize.x * mPortion.x;
    lPos.y = lSize.y * mPortion.y;
    if (mElapsed<mMaxTextTime)
        GUIMan->FillCommandQueueWithText(mFont, mText, lPos, CGUIManager::MID_CENTER, CColor(.0f, .0f, .0f));
    if ((*ActionMan)("use")->value>0.1)
        pressed = true;
    if (pressed==true && mElapsed>1.f)
        m_Finished = true;
}
