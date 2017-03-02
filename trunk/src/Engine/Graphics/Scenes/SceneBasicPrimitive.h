#ifndef _H_SCENE_Primitive__
#define _H_SCENE_Primitive__
#pragma once

#include "SceneMesh.h"

XML_FORWARD_DECLARATION

class CSceneBasicPrimitive : public CSceneMesh
{
public:
    CSceneBasicPrimitive(CXMLElement* aElement);
    virtual ~CSceneBasicPrimitive();
};

#endif
