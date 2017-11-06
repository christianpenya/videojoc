#pragma once

#ifndef _ENGINE_NodeActor_20170831_H
#define _ENGINE_NodeActor_20170831_H

#include "Events/Actor.h"

#include "GUI/GUIPosition.h"
#include "Input/ActionManager.h"

class CXMLElement;
class CSceneNode;

class CNodeInteractActor : public CActor
{

public:
    CNodeInteractActor();
    ~CNodeInteractActor();

    void Load(CXMLElement* aElement);
    void Act();
    void Update(float elapsedTime);


    CGUIManager *GUIMan;
    CRenderManager *RenderMan;
    CActionManager *ActionMan;
    std::string mText;
    std::string mFont;
    Vect2f mPortion = Vect2f(0, 0);
    float mMaxTextTime;
    float mElapsed;
    bool pressed;
};

#endif //_ENGINE_OPENDOORREACTOR_20170831_H
