#pragma once

#ifndef _ENGINE_DEACTIVATEREACTOR_20170831_H
#define _ENGINE_DEACTIVATEREACTOR_20170831_H

#include "Events/Reactor.h"

class CXMLElement;
class CSceneNode;

class CDeactivateObjReactor : public CReactor
{

public:
    CDeactivateObjReactor();
    ~CDeactivateObjReactor();

    void Load(CXMLElement* aElement);
    void React();
    void Update(float elapsedTime);

    CSceneNode* mObj;

};

#endif //_ENGINE_OPENDOORREACTOR_20170831_H
