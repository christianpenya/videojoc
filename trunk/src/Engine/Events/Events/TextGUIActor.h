#pragma once

#ifndef _ENGINE_TextGUIActor_20170831_H
#define _ENGINE_TextGUIActor_20170831_H

#include "Events/Actor.h"
#include "GUI/GUIPosition.h"

class CXMLElement;
class CSceneNode;

class CTextGUIActor : public CActor
{

public:
    CTextGUIActor();
    ~CTextGUIActor();

    void Load(CXMLElement* aElement);
    void Act();
    void Update(float elapsedTime);


    CGUIManager *GUIMan;
    CRenderManager *RenderMan;
    std::string mText;
    std::string mFont;
    Vect2f mPortion = Vect2f(0, 0);
    float mTime;
    float mTmpTime;
};

#endif //_ENGINE_OPENDOORREACTOR_20170831_H
