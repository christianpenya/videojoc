#pragma once

#ifndef _ENGINE_CIconTextReactor_20170831_H
#define _ENGINE_CIconTextReactor_20170831_H

#include "Events/Reactor.h"
#include "GUI/GUIPosition.h"

class CXMLElement;
class CSceneNode;

class CIconTextReactor : public CReactor
{

public:
    CIconTextReactor();
    ~CIconTextReactor();

    void Load(CXMLElement* aElement);
    void React();
    void Update(float elapsedTime);


    CGUIManager *GUIMan;
    CRenderManager *RenderMan;
    std::string mText;
    std::string mFont;
    Vect2f mPortion = Vect2f(0, 0);
    float mTime;
    float mTmpTime;
    std::string mIcon;
    Vect2f mPortionIcon;
};

#endif //_ENGINE_OPENDOORREACTOR_20170831_H
