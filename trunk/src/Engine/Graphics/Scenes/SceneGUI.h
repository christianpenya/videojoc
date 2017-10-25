#pragma once
#ifndef _SCENEGUI_20170110_H
#define __SCENEGUI_20170110_H_20170110_H
#include "SceneNode.h"
#include "XML/tinyxml2/tinyxml2.h"

class CSceneGUI : public CSceneNode
{
public:
    CSceneGUI(const CXMLElement* TreeNode);
    virtual ~CSceneGUI();

    bool Render(CRenderManager &RenderManager);
    bool Update(float ElapsedTime);




    Vect2f portion = Vect2f(0, 0);
    Vect2f size = Vect2f(0, 0);
    std::string gui_element = "";
    std::string nodeName = "";
    std::string type = "";
    int action = 1;


};
#endif