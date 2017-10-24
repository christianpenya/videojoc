#pragma once

#ifndef _ENGINE_RELOADSCENEREACTOR_20171020_H
#define _ENGINE_RELOADSCENEREACTOR_20171020_H

#include "Events/Reactor.h"

class CXMLElement;
class CSceneNode;

class CReloadSceneReactor : public CReactor
{

public:
    CReloadSceneReactor();
    ~CReloadSceneReactor();

    void Load(CXMLElement* aElement);
    void React();
    void Update(float elapsedTime);


};

#endif //_ENGINE_RELOADSCENEREACTOR_20171020_Hs
