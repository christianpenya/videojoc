#pragma once

#ifndef _ENGINE_TextGUIReactor_20170831_H
#define _ENGINE_TextGUIReactor_20170831_H

#include "Events/Reactor.h"
#include "GUI/GUIPosition.h"

class CXMLElement;
class CSceneNode;

class CTextGUIReactor : public CReactor
{

public:
    CTextGUIReactor();
    ~CTextGUIReactor();

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
};

#endif //_ENGINE_OPENDOORREACTOR_20170831_H
