

#include <string>
#include "XML/XML.h"
#include "Utils/Logger.h"

#include "Engine/Engine.h"

#include "Physx/PhysxManager.h"

#include "Utils/CheckedRelease.h"
#include "TextGUIReactor.h"
#include "IconTextReactor.h"

CIconTextReactor::CIconTextReactor()
{


}

CIconTextReactor::~CIconTextReactor()
{
}

void CIconTextReactor::Load(CXMLElement* aElement)
{
    GUIMan = &CEngine::GetInstance().GetGUIManager();
    RenderMan = &CEngine::GetInstance().GetRenderManager();
    mText = aElement->GetAttribute<std::string>("text", "Text Not Found :S");
    mFont = aElement->GetAttribute<std::string>("font", "font1");
    mPortion = aElement->GetAttribute<Vect2f>("portion", Vect2f(0, 0));
    mTime = aElement->GetAttribute<float>("time", 5.f);
    mIcon = aElement->GetAttribute<std::string>("icon", "icon");
    mPortionIcon = aElement->GetAttribute<Vect2f>("iPortion", Vect2f(0, 0));
}

void CIconTextReactor::React()
{
    mTmpTime = 0.0f;
}

void CIconTextReactor::Update(float elapsedTime)
{
    mTmpTime += elapsedTime;
    Vect2u lSize = RenderMan->GetWindowSize();
    Vect2f lPos;
    lPos.x = lSize.x * mPortion.x;
    lPos.y = lSize.y * mPortion.y;
    GUIMan->FillCommandQueueWithText(mFont, mText, lPos, CGUIManager::MID_CENTER, CColor(.0f, .0f, .0f));

    GUIMan->DoGUISprite(mIcon, mIcon, CGUIPosition(lSize.x*mPortionIcon.x, lSize.y*mPortionIcon.y, 50, 50));
    if (mTmpTime >= mTime)
        m_Finished = true;
}
